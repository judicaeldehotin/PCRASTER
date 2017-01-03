#include "numpy_conversion.h"
#include <boost/format.hpp>
#include <boost/mpl/if.hpp>
#include <boost/static_assert.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/type_traits.hpp>
#include <numpy/arrayobject.h>
#include "pcrtypes.h"
#include "geo_rasterspace.h"
#include "dal_Utils.h"
#include "calc_field.h"
#include "calc_spatial.h"
#include "value_scale_traits.h"


// From the numpy reference:
// http://docs.scipy.org/doc/numpy-1.6.0/reference/c-api.array.html#checking-the-api-version
//
//   ABI incompatibility is automatically detected in every numpy’s
//   version. API incompatibility detection was added in numpy 1.4.0. If
//   you want to supported many different numpy versions with one
//   extension binary, you have to build your extension with the lowest
//   NPY_FEATURE_VERSION as possible.
//
// This is the reason that we build the extension using numpy 1.4.
// - The // extension is compatible with a range of numpy versions and
// - version checks are handled by numpy.


namespace pcraster {
namespace python {

#if PY_MAJOR_VERSION >= 3
#define DEFINE_INIT_NUMPY()  \
static void* init_numpy()    \
{                            \
    import_array();          \
    return NULL;             \
}
#else
#define DEFINE_INIT_NUMPY()  \
static void init_numpy()     \
{                            \
    import_array();          \
}
#endif


void call_import_array()
{
    assert(!PyErr_Occurred());
    init_numpy();
}


bool init_numpy()
{
    assert(!PyErr_Occurred());

    static bool import_array_called = false;

    if(!import_array_called) {
        call_import_array();
        import_array_called = true;
        return !PyErr_Occurred();
    }

    assert(!PyErr_Occurred());

    return true;
}


//!
/*!
  \param     .
  \return    .
  \exception .
  \warning   .
  \sa        .
  \todo      Implement for non-spatials.
*/
boost::python::numeric::array field_to_array(
    geo::RasterSpace const& space,
    calc::Field const* field,
    double const missing_value)
{
    init_numpy();
    PRECOND(field->isSpatial());
    PRECOND(field->src());

    npy_intp dimensions[2];
    dimensions[0] = space.nrRows();
    dimensions[1] = space.nrCols();
    size_t nr_values = space.nrCells();

    switch(field->cr()) {
        case CR_UINT1: {
            boost::python::object object(boost::python::handle<>(
                PyArray_SimpleNew(2, dimensions, PyArray_UINT8)));
            char* data = ((PyArrayObject*)object.ptr())->data;
            field->beMemCpySrc(data);
            dal::fromStdMV<UINT1>((UINT1*)data, nr_values,
                static_cast<UINT1>(missing_value));
            return boost::python::extract<boost::python::numeric::array>(
                object);
            break;
        }
        case CR_INT4: {
            boost::python::object object(boost::python::handle<>(
                PyArray_SimpleNew(2, dimensions, PyArray_INT32)));
            char* data = ((PyArrayObject*)object.ptr())->data;
            field->beMemCpySrc(data);
            dal::fromStdMV<INT4>((INT4*)data, nr_values, static_cast<INT4>(
                missing_value));
            return boost::python::extract<boost::python::numeric::array>(
                object);
            break;
        }
        case CR_REAL4:
        default: {
            assert(field->cr() == CR_REAL4);
            boost::python::object object(boost::python::handle<>(
                PyArray_SimpleNew(2, dimensions, PyArray_FLOAT32)));
            char *data = ((PyArrayObject*)object.ptr())->data;
            field->beMemCpySrc(data);
            dal::fromStdMV<REAL4>((REAL4*)data, nr_values, static_cast<REAL4>(
                missing_value));
            return boost::python::extract<boost::python::numeric::array>(
                object);
            break;
        }
    }
}


#define CHECK_MINIMUM true
#define DONT_CHECK_MINIMUM false
#define CHECK_MAXIMUM true
#define DONT_CHECK_MAXIMUM false


template<
    class Source,
    class Destination,
    VS value_scale,
    bool check_minimum,
    bool check_maximum>
struct ArrayCopier
{
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct ArrayCopier<Source, Destination, value_scale,
    CHECK_MINIMUM, CHECK_MAXIMUM>
{
    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        // Source can hold destination min/max for sure.
        Source min = static_cast<Source>(
            ValueScaleTraits<value_scale>::minimum);
        Source max = static_cast<Source>(
            ValueScaleTraits<value_scale>::maximum);
        Source source_value;
        size_t const nr_values = space.nrCells();

        for(size_t i = 0; i < nr_values; ++i) {
            source_value = source[i];

            if(source_value == missing_value) {
                pcr::setMV(destination[i]);
            }
            else {
                if(source_value < min || source_value > max) {
                    typedef typename boost::mpl::if_c<
                        boost::is_same<Source, boost::uint8_t>::value ||
                        boost::is_same<Source, boost::int8_t>::value,
                    boost::int32_t, Source>::type PrintableType;
                    size_t const row = i / space.nrCols();
                    size_t const col = i - (row * space.nrCols());
                    throw std::logic_error((boost::format(
                        "Incorrect value %4% at input array [%1%][%2%] "
                        "for %3% map")
                        % row % col % ValueScaleTraits<value_scale>::name
                        % static_cast<PrintableType>(source_value)).str().c_str());
                }

                destination[i] = static_cast<Destination>(source_value);
            }
        }
    }
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct ArrayCopier<Source, Destination, value_scale,
    DONT_CHECK_MINIMUM, DONT_CHECK_MAXIMUM>
{
    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        Source source_value;
        size_t const nr_values = space.nrCells();

        for(size_t i = 0; i < nr_values; ++i) {
            source_value = source[i];

            if(source_value == missing_value) {
                pcr::setMV(destination[i]);
            }
            else {
                destination[i] = static_cast<Destination>(source_value);
            }
        }
    }
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct ArrayCopier<Source, Destination, value_scale,
    DONT_CHECK_MINIMUM, CHECK_MAXIMUM>
{
    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        // Source can hold destination max for sure.
        // (Source may not be able to hold destination min.)
        Source max = static_cast<Source>(
            ValueScaleTraits<value_scale>::maximum);
        Source source_value;
        size_t const nr_values = space.nrCells();

        for(size_t i = 0; i < nr_values; ++i) {
            source_value = source[i];

            if(source_value == missing_value) {
                pcr::setMV(destination[i]);
            }
            else {
                if(source_value > max) {
                    typedef typename boost::mpl::if_c<
                        boost::is_same<Source, boost::uint8_t>::value ||
                        boost::is_same<Source, boost::int8_t>::value,
                        boost::int32_t, Source>::type PrintableType;

                    size_t const row = i / space.nrCols();
                    size_t const col = i - (row * space.nrCols());
                    throw std::logic_error((boost::format(
                        "Incorrect value %4% at input array [%1%][%2%] "
                        "for %3% map")
                        % row % col % ValueScaleTraits<value_scale>::name
                        % static_cast<PrintableType>(source_value)).str().c_str());
                }

                destination[i] = static_cast<Destination>(source_value);
            }
        }
    }
};


#define SOURCE_IS_SMALLER_THAN_DESTINATION true
#define SOURCE_IS_NOT_SMALLER_THAN_DESTINATION false


template<
    class Source,
    class Destination,
    VS value_scale,
    bool source_is_smaller_than_destination>
struct SignedIntegralArrayToSignedIntegralArray
{
    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value);
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct SignedIntegralArrayToSignedIntegralArray<Source, Destination,
    value_scale, SOURCE_IS_SMALLER_THAN_DESTINATION>
{
    BOOST_STATIC_ASSERT(sizeof(Source) < sizeof(Destination));

    // int8, int16 -> int32
    BOOST_STATIC_ASSERT(
        (boost::is_same<Source, boost::int8_t>::value) ||
        (boost::is_same<Source, boost::int16_t>::value));
    BOOST_STATIC_ASSERT((boost::is_same<Destination, boost::int32_t>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        ArrayCopier<Source, Destination, value_scale, DONT_CHECK_MINIMUM,
            DONT_CHECK_MAXIMUM>::copy(source, destination, space,
                missing_value);
    }
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct SignedIntegralArrayToSignedIntegralArray<Source, Destination,
    value_scale, SOURCE_IS_NOT_SMALLER_THAN_DESTINATION>
{
    BOOST_STATIC_ASSERT(sizeof(Source) >= sizeof(Destination));

    // int32, int64 -> int32
    BOOST_STATIC_ASSERT(
        (boost::is_same<Source, boost::int32_t>::value) ||
        (boost::is_same<Source, boost::int64_t>::value));
    BOOST_STATIC_ASSERT((boost::is_same<Destination, boost::int32_t>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        ArrayCopier<Source, Destination, value_scale, CHECK_MINIMUM,
            CHECK_MAXIMUM>::copy(source, destination, space, missing_value);
    }
};


template<
    class Source,
    class Destination,
    VS value_scale,
    bool source_is_smaller_than_destination>
struct UnsignedIntegralArrayToSignedIntegralArray
{
    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value);
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct UnsignedIntegralArrayToSignedIntegralArray<Source, Destination,
    value_scale, SOURCE_IS_SMALLER_THAN_DESTINATION>
{
    // uint8, uint16 -> int32
    BOOST_STATIC_ASSERT(
        (boost::is_same<Source, boost::uint8_t>::value) ||
        (boost::is_same<Source, boost::uint16_t>::value));
    BOOST_STATIC_ASSERT((boost::is_same<Destination, boost::int32_t>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        ArrayCopier<Source, Destination, value_scale, DONT_CHECK_MINIMUM,
            DONT_CHECK_MAXIMUM>::copy(
            source, destination, space, missing_value);
    }
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct UnsignedIntegralArrayToSignedIntegralArray<Source, Destination,
    value_scale, SOURCE_IS_NOT_SMALLER_THAN_DESTINATION>
{
    // uint32, uint64 -> int32
    BOOST_STATIC_ASSERT(
        (boost::is_same<Source, boost::uint32_t>::value) ||
        (boost::is_same<Source, boost::uint64_t>::value));
    BOOST_STATIC_ASSERT((boost::is_same<Destination, boost::int32_t>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        ArrayCopier<Source, Destination, value_scale, DONT_CHECK_MINIMUM,
            CHECK_MAXIMUM>::copy(
            source, destination, space, missing_value);
    }
};


#undef SOURCE_IS_SMALLER_THAN_DESTINATION
#undef SOURCE_IS_NOT_SMALLER_THAN_DESTINATION
#define SOURCE_IS_SIGNED true
#define SOURCE_IS_UNSIGNED false
#define DESTINATION_IS_SIGNED true
#define DESTINATION_IS_UNSIGNED false


template<
    class Source,
    class Destination,
    VS value_scale,
    bool source_is_signed,
    bool destination_is_signed>
struct IntegralArrayToIntegralArray
{
    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value);
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct IntegralArrayToIntegralArray<Source, Destination, value_scale,
    SOURCE_IS_SIGNED, DESTINATION_IS_SIGNED>
{
    // int -> int32
    BOOST_STATIC_ASSERT((boost::is_same<Destination, boost::int32_t>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        // int -> int32
        SignedIntegralArrayToSignedIntegralArray<Source, Destination,
            value_scale, sizeof(Source) < sizeof(Destination)>::copy(
                source, destination, space, missing_value);
    }
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct IntegralArrayToIntegralArray<Source, Destination, value_scale,
    SOURCE_IS_UNSIGNED, DESTINATION_IS_UNSIGNED>
{
    // uint -> uint8
    BOOST_STATIC_ASSERT((boost::is_same<Destination, boost::uint8_t>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        ArrayCopier<Source, Destination, value_scale, CHECK_MINIMUM,
            CHECK_MAXIMUM>::copy(source, destination, space, missing_value);
    }
};



template<
    class Source,
    class Destination,
    VS value_scale>
struct IntegralArrayToIntegralArray<Source, Destination, value_scale,
    SOURCE_IS_SIGNED, DESTINATION_IS_UNSIGNED>
{
    // int -> uint8
    BOOST_STATIC_ASSERT((boost::is_same<Destination, boost::uint8_t>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        ArrayCopier<Source, Destination, value_scale, CHECK_MINIMUM,
            CHECK_MAXIMUM>::copy(source, destination, space, missing_value);
    }
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct IntegralArrayToIntegralArray<Source, Destination, value_scale,
    SOURCE_IS_UNSIGNED, DESTINATION_IS_SIGNED>
{
    // uint -> int32
    BOOST_STATIC_ASSERT((boost::is_same<Destination, boost::int32_t>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        UnsignedIntegralArrayToSignedIntegralArray<Source, Destination,
            value_scale, sizeof(Source) < sizeof(Destination)>::copy(
                source, destination, space, missing_value);
    }
};


#undef SOURCE_IS_SIGNED
#undef SOURCE_IS_UNSIGNED
#undef DESTINATION_IS_SIGNED
#undef DESTINATION_IS_UNSIGNED
#define SOURCE_EQUALS_DESTINATION true
#define SOURCE_DOESNT_EQUAL_DESTINATION false


template<
    class Source,
    class Destination,
    VS value_scale,
    bool source_equals_destination>
struct FloatArrayToFloatArray
{
    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value);
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct FloatArrayToFloatArray<Source, Destination, value_scale,
    SOURCE_EQUALS_DESTINATION>
{
    // float32 -> float32
    BOOST_STATIC_ASSERT((boost::is_same<Source, float>::value));
    BOOST_STATIC_ASSERT((boost::is_same<Destination, float>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        // TODO Possible optimization: memcpy values and replace missing_value
        //      in destination.
        ArrayCopier<Source, Destination, value_scale, DONT_CHECK_MINIMUM,
            DONT_CHECK_MAXIMUM>::copy(source, destination, space,
                missing_value);
    }
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct FloatArrayToFloatArray<Source, Destination, value_scale,
    SOURCE_DOESNT_EQUAL_DESTINATION>
{
    // float64 -> float32
    BOOST_STATIC_ASSERT((boost::is_same<Source, double>::value));
    BOOST_STATIC_ASSERT((boost::is_same<Destination, float>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        ArrayCopier<Source, Destination, value_scale, DONT_CHECK_MINIMUM,
            DONT_CHECK_MAXIMUM>::copy(source, destination, space,
                missing_value);
    }
};


#undef SOURCE_EQUALS_DESTINATION
#undef SOURCE_DOESNT_EQUAL_DESTINATION
#define DESTINATION_IS_SIGNED true
#define DESTINATION_IS_UNSIGNED false

template<
    class Source,
    class Destination,
    VS value_scale,
    bool destination_is_signed>
struct FloatArrayToIntegralArray
{
    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value);
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct FloatArrayToIntegralArray<Source, Destination, value_scale,
    DESTINATION_IS_SIGNED>
{
    // float -> int32
    BOOST_STATIC_ASSERT((boost::is_same<Destination, boost::int32_t>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        ArrayCopier<Source, Destination, value_scale, CHECK_MINIMUM,
            CHECK_MAXIMUM>::copy(source, destination, space, missing_value);
    }
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct FloatArrayToIntegralArray<Source, Destination, value_scale,
    DESTINATION_IS_UNSIGNED>
{
    // float -> uint8
    BOOST_STATIC_ASSERT((boost::is_same<Destination, boost::uint8_t>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        ArrayCopier<Source, Destination, value_scale, CHECK_MINIMUM,
            CHECK_MAXIMUM>::copy(source, destination, space, missing_value);
    }
};


#undef DESTINATION_IS_SIGNED
#undef DESTINATION_IS_UNSIGNED
#define SOURCE_IS_INTEGRAL true
#define SOURCE_IS_FLOAT false
#define DESTINATION_IS_INTEGRAL true
#define DESTINATION_IS_FLOAT false


template<
    class Source,
    class Destination,
    VS value_scale,
    bool source_is_integral,
    bool destination_is_integral>
struct ArrayToArray
{
    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value);
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct ArrayToArray<Source, Destination, value_scale,
    SOURCE_IS_INTEGRAL, DESTINATION_IS_INTEGRAL>
{
    // int, uint -> uint8, int32
    BOOST_STATIC_ASSERT(
        (boost::is_same<Destination, boost::uint8_t>::value) ||
        (boost::is_same<Destination, boost::int32_t>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        IntegralArrayToIntegralArray<Source, Destination, value_scale,
            boost::is_signed<Source>::value,
            boost::is_signed<Destination>::value>::copy(source, destination,
                space, missing_value);
    }
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct ArrayToArray<Source, Destination, value_scale,
    SOURCE_IS_FLOAT, DESTINATION_IS_FLOAT>
{
    // float -> float32
    BOOST_STATIC_ASSERT((boost::is_same<Destination, float>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        FloatArrayToFloatArray<Source, Destination, value_scale,
            boost::is_same<Source, Destination>::value>::copy(source,
                destination, space, missing_value);
    }
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct ArrayToArray<Source, Destination, value_scale,
    SOURCE_IS_INTEGRAL, DESTINATION_IS_FLOAT>
{
    // int -> float32
    BOOST_STATIC_ASSERT((boost::is_same<Destination, float>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        ArrayCopier<Source, Destination, value_scale, DONT_CHECK_MINIMUM,
            DONT_CHECK_MAXIMUM>::copy(source, destination, space,
                missing_value);
    }
};


template<
    class Source,
    class Destination,
    VS value_scale>
struct ArrayToArray<Source, Destination, value_scale,
    SOURCE_IS_FLOAT, DESTINATION_IS_INTEGRAL>
{
    // float -> uint8, int32
    BOOST_STATIC_ASSERT(
        (boost::is_same<Destination, boost::uint8_t>::value) ||
        (boost::is_same<Destination, boost::int32_t>::value));

    static void copy(
        Source const* source,
        Destination* destination,
        geo::RasterSpace const& space,
        Source const missing_value)
    {
        FloatArrayToIntegralArray<Source, Destination, value_scale,
            boost::is_signed<Destination>::value>::copy(source, destination,
                space, missing_value);
    }
};


#undef SOURCE_IS_INTEGRAL
#undef SOURCE_IS_FLOAT
#undef DESTINATION_IS_INTEGRAL
#undef DESTINATION_IS_FLOAT
#undef CHECK_MINIMUM
#undef DONT_CHECK_MINIMUM
#undef CHECK_MAXIMUM
#undef DONT_CHECK_MAXIMUM


template<
    class Source,
    VS value_scale>
calc::Spatial* array_to_field(
    geo::RasterSpace const& space,
    boost::python::numeric::array const& array,
    Source const missing_value)
{
    calc::Spatial* field = new calc::Spatial(value_scale,
        ValueScaleTraits<value_scale>::cell_representation_index,
        space.nrCells());

    typedef typename ValueScaleTraits<value_scale>::Type Destination;
    Source const* source = static_cast<Source const*>(
        PyArray_DATA(array.ptr()));
    Destination* destination = static_cast<Destination*>(field->dest());

    try {
        // Select the minimum amount of logic to convert the array value
        // correctly to field values. At compile time.
        ArrayToArray<Source, Destination, value_scale,
            boost::is_integral<Source>::value,
            boost::is_integral<Destination>::value>::copy(
                source, destination, space, missing_value);
    }
    catch(...) {
        delete field;
        throw;
    }

    return field;
}


#define ARRAY_TO_FIELD_CASE(source_type, value_scale)                          \
    case value_scale: {                                                        \
        field = array_to_field<source_type, value_scale>(                      \
            space, array, static_cast<source_type>(missing_value));            \
        break;                                                                 \
    }


#define ARRAY_TO_FIELD(source_type, value_scale)                               \
    switch(value_scale) {                                                      \
        ARRAY_TO_FIELD_CASE(source_type, VS_B)                                 \
        ARRAY_TO_FIELD_CASE(source_type, VS_L)                                 \
        ARRAY_TO_FIELD_CASE(source_type, VS_N)                                 \
        ARRAY_TO_FIELD_CASE(source_type, VS_O)                                 \
        ARRAY_TO_FIELD_CASE(source_type, VS_S)                                 \
        ARRAY_TO_FIELD_CASE(source_type, VS_D)                                 \
        default: {                                                             \
            assert(false);                                                     \
            break;                                                             \
        }                                                                      \
    }


calc::Field* array_to_field(
    geo::RasterSpace const& space,
    VS const value_scale,
    boost::python::numeric::array const& array,
    double const missing_value)
{
    init_numpy();

    if(!space.valid()) {
        throw std::logic_error(
            "No valid raster defined: Set clone or load map from file");
    }
    if((((PyArrayObject*)array.ptr())->nd) != 2) {
        throw std::logic_error(
            "Dimension error: Rank of input array must be 2");
    }

    size_t const nr_cells_in_array =
        ((PyArrayObject*)array.ptr())->dimensions[0] *
        ((PyArrayObject*)array.ptr())->dimensions[1];
    size_t const nr_cells_in_field = space.nrCells();

    if(nr_cells_in_array != nr_cells_in_field){
        throw std::logic_error((boost::format(
            "Size mismatch: Number of array elements is %1%, number of raster "
            "cells is %2%")
            % nr_cells_in_array
            % nr_cells_in_field).str().c_str());
    }

    int const type = PyArray_TYPE(array.ptr());
    calc::Spatial* field = NULL;

    // http://docs.scipy.org/doc/numpy/reference/c-api.dtype.html
    switch(type) {
        case NPY_BOOL:
        case NPY_INT8: {
            ARRAY_TO_FIELD(boost::int8_t, value_scale)
            break;
        }
        case NPY_UINT8: {
            ARRAY_TO_FIELD(boost::uint8_t, value_scale)
            break;
        }
        case NPY_INT16: {
            ARRAY_TO_FIELD(boost::int16_t, value_scale)
            break;
        }
        case NPY_UINT16: {
            ARRAY_TO_FIELD(boost::uint16_t, value_scale)
            break;
        }
        case NPY_INT32: {
            ARRAY_TO_FIELD(boost::int32_t, value_scale)
            break;
        }
        case NPY_UINT32: {
            ARRAY_TO_FIELD(boost::uint32_t, value_scale)
            break;
        }
        case NPY_INT64: {
            ARRAY_TO_FIELD(boost::int64_t, value_scale)
            break;
        }
        case NPY_UINT64: {
            ARRAY_TO_FIELD(boost::uint64_t, value_scale)
            break;
        }
        case NPY_FLOAT32: {
            ARRAY_TO_FIELD(float, value_scale)
            break;
        }
        case NPY_FLOAT64: {
            ARRAY_TO_FIELD(double, value_scale)
            break;
        }
        default: {
            throw std::logic_error((boost::format(
                "Unsupported array type")).str().c_str());
        }
    }

    return field;
}

#undef ARRAY_TO_FIELD
#undef ARRAY_TO_FIELD_CASE


//! Return a numpy array that references the original buffer of data.
/*!
  \param     .
  \return    .
  \exception .
  \warning   .
  \sa        .
*/
boost::python::numeric::array field_as_array(
    geo::RasterSpace const& space,
    PyObject* field_object)
{
    init_numpy();
    boost::python::extract<calc::Field*> field_extractor(field_object);

    if(!field_extractor.check()) {
         throw std::logic_error("Expecting a PCRaster field");
    }

    calc::Field* field(field_extractor());
    assert(field);

    PRECOND(field->isSpatial());
    PRECOND(field->src());

    npy_intp dimensions[2];
    dimensions[0] = space.nrRows();
    dimensions[1] = space.nrCols();
    PyObject* array = 0;

    switch(field->cr()) {
      case CR_UINT1: {
        array = PyArray_SimpleNewFromData(2, dimensions, PyArray_UINT8,
            const_cast<void*>(field->src()));
        break;
      }
      case CR_INT4: {
        array = PyArray_SimpleNewFromData(2, dimensions, PyArray_INT32,
            const_cast<void*>(field->src()));
        break;
      }
      case CR_REAL4:
      default: {
        assert(field->cr() == CR_REAL4);
        array = PyArray_SimpleNewFromData(2, dimensions, PyArray_FLOAT32,
            const_cast<void*>(field->src()));
        break;
      }
    }

    assert(array);
    PyArray_BASE(array) = field_object;
    Py_INCREF(field_object);
    return boost::python::extract<boost::python::numeric::array>(array);
}

} // namespace python
} // namespace pcraster
