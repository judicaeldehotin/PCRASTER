#ifndef INCLUDED_CALC_DATASTORAGEID
#define INCLUDED_CALC_DATASTORAGEID



#ifndef INCLUDED_STDDEFX
#include "stddefx.h"
#define INCLUDED_STDDEFX
#endif

// Library headers.
#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

// PCRaster library headers.
#include "pcraster_model_engine_export.h"

// Module headers.
#ifndef INCLUDED_CALC_DATAVALUE
#include "calc_datavalue.h"
#define INCLUDED_CALC_DATAVALUE
#endif



namespace calc {
  // DataStorageId declarations.
}



namespace calc {



//! Identification of DataStorage
/*!
   typically a filename
*/
class PCR_ME_EXPORT DataStorageId: public DataValue
{

  std::string      d_id;


private:

  //! Assignment operator. NOT IMPLEMENTED.
  DataStorageId&           operator=           (DataStorageId const& rhs);

  //! Copy constructor. NOT IMPLEMENTED.
                   DataStorageId               (DataStorageId const& rhs);

public:

  //----------------------------------------------------------------------------
  // CREATORS
  //----------------------------------------------------------------------------

                   DataStorageId               (const std::string& id);

  /* virtual */    ~DataStorageId              () override;

  //----------------------------------------------------------------------------
  // MANIPULATORS
  //----------------------------------------------------------------------------
  void               setId                     (const std::string& id);

  //----------------------------------------------------------------------------
  // ACCESSORS
  //----------------------------------------------------------------------------
  OVS                ovs                       () const override;
  const std::string& id                        () const;

};



//------------------------------------------------------------------------------
// INLINE FUNCTIONS
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// FREE OPERATORS
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// FREE FUNCTIONS
//------------------------------------------------------------------------------



} // namespace calc

#endif
