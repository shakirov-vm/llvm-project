#include "RISCSimRegisterInfo.h"
#include "RISCSim.h"

#include "RISCSimInstrInfo.h"
//#include "RISCSimMachineFunctionInfo.h"
#include "RISCSimSubtarget.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

#define DEBUG_TYPE "RISCSim-reg-info"

#define GET_REGINFO_TARGET_DESC
#include "RISCSimGenRegisterInfo.inc"

RISCSimRegisterInfo::RISCSimRegisterInfo() : RISCSimGenRegisterInfo(RISCSim::x0) {}

#if 0
bool RISCSimRegisterInfo::needsFrameMoves(const MachineFunction &MF) {
  return MF.needsFrameMoves();
}
#endif

const MCPhysReg *
RISCSimRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_RISCSim_SaveList;
}

// TODO: check cconv
BitVector RISCSimRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  RISCSimFrameLowering const *TFI = getFrameLowering(MF);

  BitVector Reserved(getNumRegs());
  Reserved.set(RISCSim::x6); // SP

  if (TFI->hasFP(MF)) {
    Reserved.set(RISCSim::x5); // FP
  }
  return Reserved;
}

bool RISCSimRegisterInfo::requiresRegisterScavenging(
    const MachineFunction &MF) const {
  return false; // TODO: what for?
}

#if 0
bool RISCSimRegisterInfo::useFPForScavengingIndex(
    const MachineFunction &MF) const {
  llvm_unreachable("");
}
#endif

// TODO: rewrite!
bool RISCSimRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                           int SPAdj, unsigned FIOperandNum,
                                           RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected non-zero SPAdj value");

  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  DebugLoc DL = MI.getDebugLoc();

  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  Register FrameReg;
  int Offset = getFrameLowering(MF)
                   ->getFrameIndexReference(MF, FrameIndex, FrameReg)
                   .getFixed();
  Offset += MI.getOperand(FIOperandNum + 1).getImm();

  if (!isInt<16>(Offset)) {
    llvm_unreachable("");
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, false, false, false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
  return false;
}

Register RISCSimRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const TargetFrameLowering *TFI = getFrameLowering(MF);
  return TFI->hasFP(MF) ? RISCSim::x5 /* FP */ : RISCSim::x6 /* SP */;
}

const uint32_t *
RISCSimRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID CC) const {
  return CSR_RISCSim_RegMask;
}