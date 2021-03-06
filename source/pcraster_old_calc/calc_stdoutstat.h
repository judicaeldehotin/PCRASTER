#ifndef INCLUDED_STDOUTSTATEMENT
#define INCLUDED_STDOUTSTATEMENT

#ifndef INCLUDED_CALC_STATEMENT
# include "calc_statement.h"
#define INCLUDED_CALC_STATEMENT
#endif

#ifndef INCLUDED_MEMORY
# include <memory>
#define INCLUDED_MEMORY
#endif



namespace calc {

class FieldExpr;

//! an expression that print to stdout
class StdoutStatement : public Statement {
  std::unique_ptr<FieldExpr> d_expr;
 public:
  // CREATORS
  StdoutStatement(
    FieldExpr *right);

  bool buildTypes() override;
  void prepareExecution() override;
  void run() override;

  // ACCESSORS
  void print(InfoScript& i)const override;
};

}

#endif
