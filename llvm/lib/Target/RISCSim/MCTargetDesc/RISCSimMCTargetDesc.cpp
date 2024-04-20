#include "RISCSimMCTargetDesc.h"

#include "RISCSim.h"
#include "TargetInfo/RISCSimTargetInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_REGINFO_MC_DESC
#include "RISCSimGenRegisterInfo.inc"

static MCRegisterInfo *createRISCSimMCRegisterInfo(const Triple &TT) {
  RISCSIM_DUMP_MAGENTA
  MCRegisterInfo *X = new MCRegisterInfo();
  InitRISCSimMCRegisterInfo(X, RISCSim::x0);
  return X;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRISCSimTargetMC() {
  RISCSIM_DUMP_MAGENTA
  Target &TheRISCSimTarget = getTheRISCSimTarget();
  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(TheRISCSimTarget, createRISCSimMCRegisterInfo);
}
