#ifndef INCLUDED_DAL_CONSTANTDRIVER
#define INCLUDED_DAL_CONSTANTDRIVER



// External headers.

// Project headers.

// Module headers.
#ifndef INCLUDED_DAL_DRIVER
#include "dal_Driver.h"
#define INCLUDED_DAL_DRIVER
#endif

#ifndef INCLUDED_DAL_CONSTANT
#include "dal_Constant.h"
#define INCLUDED_DAL_CONSTANT
#endif



namespace dal {
  // ConstantDriver declarations.
}



namespace dal {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class ConstantDriver: public Driver
{

  friend class ConstantDriverTest;

private:

protected:

                   ConstantDriver      (Format const& format);


public:

  //----------------------------------------------------------------------------
  // CREATORS
  //----------------------------------------------------------------------------

  /* virtual */    ~ConstantDriver     () override;

  //----------------------------------------------------------------------------
  // MANIPULATORS
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // ACCESSORS
  //----------------------------------------------------------------------------

  bool             exists              (std::string const& name) const;

  bool     exists              (std::string const& name,
                                        dal::DataSpace const& space,
                                        dal::DataSpaceAddress const& address) const override =0;

  Constant*        open                (std::string const& name) const;

  Constant* open               (std::string const& name,
                                        dal::DataSpace const& space,
                                        dal::DataSpaceAddress const& address) const override =0;

  DataSpace        dataSpace           (std::string const& name,
                                        DataSpace const& space,
                                        DataSpaceAddress const& address) const override;

  Constant*        read                (std::string const& name) const override;

  Constant* read               (std::string const& name,
                                        dal::DataSpace const& space,
                                        dal::DataSpaceAddress const& address) const override =0;

  void             read                (void* cell,
                                        TypeId typeId,
                                        std::string const& name,
                                        DataSpace const& space,
                                        DataSpaceAddress const& address) const override;

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

} // namespace dal

#endif
