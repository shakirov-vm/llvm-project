
//===----------------------------------------------------------------------===//
// Implements the info about RISCSim target spec.
//===----------------------------------------------------------------------===//

#include "RISCSimTargetMachine.h"
#include "TargetInfo/RISCSimTargetInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/Scalar.h"

//#include "RISCSimTargetTransformInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"

#include "llvm/Support/CodeGen.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetOptions.h"

#define DEBUG_TYPE "RISCSim"

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRISCSimTarget() {
  // Register the target.
  RISCSIM_DUMP_CYAN
  RegisterTargetMachine<RISCSimTargetMachine> A(getTheRISCSimTarget());
}

RISCSimTargetMachine::RISCSimTargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   std::optional<Reloc::Model> RM,
                                   std::optional<CodeModel::Model> CM,
                                   CodeGenOptLevel OL, bool JIT)
    : LLVMTargetMachine(T, "e-m:e-p:32:32-i8:8:32-i16:16:32-i64:64-n32", TT,
                        CPU, FS, Options, Reloc::Static,
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, std::string(CPU), std::string(FS), *this) {
  initAsmInfo();
}

RISCSimTargetMachine::~RISCSimTargetMachine() = default;

namespace {

/// RISCSim Code Generator Pass Configuration Options.
class RISCSimPassConfig : public TargetPassConfig {
public:
  RISCSimPassConfig(RISCSimTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  RISCSimTargetMachine &getRISCSimTargetMachine() const {
    return getTM<RISCSimTargetMachine>();
  }

  bool addInstSelector() override;
};

} // end anonymous namespace

TargetPassConfig *RISCSimTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new RISCSimPassConfig(*this, PM);
}

bool RISCSimPassConfig::addInstSelector() {
  addPass(createRISCSimISelDag(getRISCSimTargetMachine()));
  return false;
}
