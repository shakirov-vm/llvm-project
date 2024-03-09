#include "TargetInfo/RISCSimTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
using namespace llvm;

Target &llvm::getTheRISCSimTarget() {
  static Target TheRISCSimTarget;
  return TheRISCSimTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRISCSimTargetInfo() {
  RegisterTarget<Triple::riscsim> X(getTheRISCSimTarget(), "riscsim", "RISCSim 32",
                                   "RISCSIM");
}