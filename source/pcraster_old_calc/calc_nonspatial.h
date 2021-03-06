#ifndef INCLUDED_CALC_NONSPATIAL
#define INCLUDED_CALC_NONSPATIAL

#ifndef INCLUDED_CSFTYPES
#include "csftypes.h"
#define INCLUDED_CSFTYPES
#endif

#ifndef INCLUDED_CALC_FIELD
#include "calc_field.h"
#define INCLUDED_CALC_FIELD
#endif

namespace calc {

//! holds data for a non-spatial value
class NonSpatial : public Field {
  //! which d_val field holds value?
  CSF_CR  d_crVal;

  REAL4  d_vals;
  INT4   d_val4;
  UINT1  d_val1;
  const void *voidValue() const;
  public:
  // CREATORS

  //! initialization with a value
  NonSpatial(VS vs, double value=0);

  ~NonSpatial() override;

  // ACCESSORS

  //! is it a mv
  bool isMv() const override;
  //! return value as double, should not be a MV
  double getValue() const;
  void   setCell(const double& value, size_t /* i */) override;
  bool   getCell(double& value, size_t /* i */) const override;
  NonSpatial *copy() const override;

  size_t              nrValues() const override;

  bool                isSpatial() const override;
  const void*         srcValue() const override;
  void*               destValue() override;
  void                analyzeBoolean(bool& noneAreTrue,bool& noneAreFalse) const override;
};

}

#endif
