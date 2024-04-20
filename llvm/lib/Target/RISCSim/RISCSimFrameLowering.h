#ifndef LLVM_LIB_TARGET_RISCSIM_SIMFRAMELOWERING_H
#define LLVM_LIB_TARGET_RISCSIM_SIMFRAMELOWERING_H

#include "RISCSim.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {

class RISCSimFrameLowering : public TargetFrameLowering {
public:
  explicit RISCSimFrameLowering()
      : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(4), 0) {
    RISCSIM_DUMP_GREEN
  }

  /// emitProlog/emitEpilog - These methods insert prolog and epilog code into
  /// the function.
  void emitPrologue(MachineFunction &MF,
                    MachineBasicBlock &MBB) const override {}
  void emitEpilogue(MachineFunction &MF,
                    MachineBasicBlock &MBB) const override {}

  /// hasFP - Return true if the specified function should have a dedicated
  /// frame pointer register. For most targets this is true only if the function
  /// has variable sized allocas or if frame pointer elimination is disabled.
  bool hasFP(const MachineFunction &MF) const override { return false; }
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_RISCSIM_SIMFRAMELOWERING_H
