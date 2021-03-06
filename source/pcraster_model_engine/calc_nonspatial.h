#ifndef INCLUDED_CALC_NONSPATIAL
#define INCLUDED_CALC_NONSPATIAL

#ifndef INCLUDED_CSFTYPES
#include "csftypes.h"
#define INCLUDED_CSFTYPES
#endif

#include "pcraster_model_engine_export.h"

#ifndef INCLUDED_CALC_FIELD
#include "calc_field.h"
#define INCLUDED_CALC_FIELD
#endif

namespace calc {

//! holds data for a non-spatial value
class PCR_ME_EXPORT NonSpatial : public Field {

  union {
  REAL4  d_valS[1];
  REAL4  d_vals;
  INT4   d_val4;
  UINT1  d_val1;
  };
  const void *voidValue() const;
  public:
  // CREATORS

  //! initialization with a value
  /*!
   * \todo
   *   get rid of this one OR the template version,
   *   difference is tricky
   */
  NonSpatial(VS vs, double value=0);

  NonSpatial(const NonSpatial& rhs);

  //! initialization with a value
  template<typename CR>
  PCR_ME_EXPORT NonSpatial(VS vs, const CR& value);

  ~NonSpatial() override;

  // ACCESSORS

  //! is it a mv
  bool        isMV() const override;

  //! return value as double, should not be a MV
  double      getValue() const;
  void        setCell(const double& value, size_t /* i */) override;
  bool        getCell(double& value, size_t /* i */) const override;

  size_t              nrValues() const override;

  bool                isSpatial() const override;
  const void*         src() const override;
  void*               dest() override ;
  void                analyzeBoolean(bool& noneAreTrue,bool& noneAreFalse) const override;
  NonSpatial*         createClone() const override;
};


template<typename CR>
NonSpatial *createNonSpatial(VS vs, const CR& v) {
    CR val=v;
    return new NonSpatial(vs,val);
}

}

#endif
