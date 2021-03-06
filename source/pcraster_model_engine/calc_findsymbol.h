#ifndef INCLUDED_CALC_FINDSYMBOL
#define INCLUDED_CALC_FINDSYMBOL

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
#ifndef INCLUDED_CALC_TYPES
#include "calc_types.h"
#define INCLUDED_CALC_TYPES
#endif

#ifndef INCLUDED_MAJOR_OP
#include "major_op.h"
#define INCLUDED_MAJOR_OP
#endif



namespace calc {
  // FindSymbol declarations.
  class ModelLink;
  class Operator;
}



namespace calc {

PCR_ME_EXPORT const Operator* major2op     (MAJOR_CODE major);
PCR_ME_EXPORT const Operator* opName2op    (const std::string& opName,size_t nrArgs=2);

PCR_ME_EXPORT void     loadCalcLib         (const std::string& dllName);

size_t             nrInternalOpCodes   ();


void               externalBuildType   (VS& newVs,
                                        bool& isSpatial,
                                        MAJOR_CODE major,
                                        const OP_ARGS* args,
                                        int nrArgs);

int                externalExecute     (MAJOR_CODE major,
                                        void **out,
                                        const void **in,
                                        int nrArgs);

ModelLink*         createModelLink     (const std::string& name);

} // namespace calc

#endif
