#include "RISCSimSubtarget.h"
#include "RISCSim.h"

#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define DEBUG_TYPE "RISCSim-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "RISCSimGenSubtargetInfo.inc"

void RISCSimSubtarget::anchor() {}

RISCSimSubtarget::RISCSimSubtarget(const Triple &TT, const std::string &CPU,
                             const std::string &FS, const TargetMachine &TM)
    : RISCSimGenSubtargetInfo(TT, CPU, /*TuneCPU=*/CPU, FS), InstrInfo(),
      FrameLowering(*this), TLInfo(TM, *this) {}