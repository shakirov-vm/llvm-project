#include "RISCSimMCTargetDesc.h"

#include "RISCSim.h"
#include "TargetInfo/RISCSimTargetInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_REGINFO_MC_DESC
#include "RISCSimGenRegisterInfo.inc"

#define GET_INSTRINFO_MC_DESC
#include "RISCSimGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "RISCSimGenSubtargetInfo.inc"

static MCRegisterInfo *createRISCSimMCRegisterInfo(const Triple &TT) {
  RISCSIM_DUMP_MAGENTA
  MCRegisterInfo *X = new MCRegisterInfo();
  InitRISCSimMCRegisterInfo(X, RISCSim::x0);
  return X;
}

static MCInstrInfo *createRISCSimMCInstrInfo() {
  RISCSIM_DUMP_MAGENTA
  MCInstrInfo *X = new MCInstrInfo();
  InitRISCSimMCInstrInfo(X);
  return X;
}

static MCSubtargetInfo *createRISCSimMCSubtargetInfo(const Triple &TT,
                                                 StringRef CPU, StringRef FS) {
  RISCSIM_DUMP_MAGENTA
  return createRISCSimMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRISCSimTargetMC() {
  RISCSIM_DUMP_MAGENTA
  Target &TheRISCSimTarget = getTheRISCSimTarget();
  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(TheRISCSimTarget, createRISCSimMCRegisterInfo);
  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(TheRISCSimTarget, createRISCSimMCInstrInfo);
  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(TheRISCSimTarget, createRISCSimMCSubtargetInfo);
}
