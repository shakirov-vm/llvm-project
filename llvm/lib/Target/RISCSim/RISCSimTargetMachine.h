
#ifndef LLVM_LIB_TARGET_RISCSIM_RISCSIMTARGETMACHINE_H
#define LLVM_LIB_TARGET_RISCSIM_RISCSIMTARGETMACHINE_H

#include "llvm/Support/CodeGen.h"
#include "llvm/Target/TargetMachine.h"
#include <memory>

#include "RISCSimInstrInfo.h"
#include "RISCSimSubtarget.h"

namespace llvm {

class RISCSimTargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  RISCSimSubtarget Subtarget;

public:
  RISCSimTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                     StringRef FS, const TargetOptions &Options,
                     std::optional<Reloc::Model> RM,
                     std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                     bool JIT);
  ~RISCSimTargetMachine() override;

  const RISCSimSubtarget *getSubtargetImpl(const Function &) const override {
    return &Subtarget;
  }

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
};
} // end namespace llvm

#endif // LLVM_LIB_TARGET_RISCSIM_RISCSIMTARGETMACHINE_H