#include "RISCSimSubtarget.h"
#include "RISCSim.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "RISCsim-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "RISCSimGenSubtargetInfo.inc"

RISCSimSubtarget::RISCSimSubtarget(const StringRef &CPU, const StringRef &TuneCPU,
                           const StringRef &FS, const TargetMachine &TM)
    : RISCSimGenSubtargetInfo(TM.getTargetTriple(), CPU, TuneCPU, FS) {
  RISCSIM_DUMP_CYAN
}
