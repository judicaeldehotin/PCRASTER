#ifndef INCLUDED_CALC_CODE
#define INCLUDED_CALC_CODE



#ifndef INCLUDED_STDDEFX
#include "stddefx.h"
#define INCLUDED_STDDEFX
#endif

// Library headers.

// PCRaster library headers.

// Module headers.
#ifndef INCLUDED_CALC_BASICBLOCK
#include "calc_basicblock.h"
#define INCLUDED_CALC_BASICBLOCK
#endif


namespace calc {
  // Code declarations.
}



namespace calc {



//! complete code
class Code : public BasicBlock
{

private:

  //! Assignment operator. NOT IMPLEMENTED.
  Code&           operator=           (const Code& rhs);

  //! Copy constructor. NOT IMPLEMENTED.
                   Code               (const Code& rhs);

public:

  //----------------------------------------------------------------------------
  // CREATORS
  //----------------------------------------------------------------------------

                   Code               (ASTNode*       transferredStatements);

  /* virtual */    ~Code              () override;

  //----------------------------------------------------------------------------
  // MANIPULATORS
  //----------------------------------------------------------------------------
  void            callEnter                     (ASTVisitor& v) override;
  void            callJump                      (ASTVisitor& v) override;

  //----------------------------------------------------------------------------
  // ACCESSORS
  //----------------------------------------------------------------------------
  Code*           createClone                   () const override;
  bool            hasBackBranch                 () const override;
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
