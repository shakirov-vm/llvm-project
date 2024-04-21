#include "RISCSimFrameLowering.h"
#include "RISCSimMachineFunctionInfo.h"
#include "RISCSimSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include <algorithm>

#define DEBUG_TYPE "RISCSim-frame-lowering"

using namespace llvm;

// seems done
void RISCSimFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                             BitVector &SavedRegs,
                                             RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
  // Unconditionally spill RA and FP only if the function uses a frame
  // pointer.
  if (hasFP(MF)) {
    SavedRegs.set(RISCSim::x4); // RA
    SavedRegs.set(RISCSim::x5); // FP
  }
}

void RISCSimFrameLowering::adjustStackToMatchRecords(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI,
    bool Allocate) const {
  llvm_unreachable("");
}

// seems ok
void RISCSimFrameLowering::emitPrologue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
  return;
}

// seems ok
void RISCSimFrameLowering::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
  return;
}

// seems ok
bool RISCSimFrameLowering::spillCalleeSavedRegisters(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
    ArrayRef<CalleeSavedInfo> CSI, const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return true;

  MachineFunction *MF = MBB.getParent();
  const TargetInstrInfo &TII = *MF->getSubtarget().getInstrInfo();

  for (auto &CS : CSI) {
    // Insert the spill to the stack frame.
    Register Reg = CS.getReg();
    const TargetRegisterClass *RC = TRI->getMinimalPhysRegClass(Reg);
    TII.storeRegToStackSlot(MBB, MI, Reg, !MBB.isLiveIn(Reg), CS.getFrameIdx(),
                            RC, TRI, 0);
  }

  return true;
}

// seems ok
bool RISCSimFrameLowering::restoreCalleeSavedRegisters(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
    MutableArrayRef<CalleeSavedInfo> CSI, const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return true;

  MachineFunction *MF = MBB.getParent();
  const TargetInstrInfo &TII = *MF->getSubtarget().getInstrInfo();

  // Insert in reverse order.
  // loadRegFromStackSlot can insert multiple instructions.
  for (auto &CS : reverse(CSI)) {
    Register Reg = CS.getReg();
    const TargetRegisterClass *RC = TRI->getMinimalPhysRegClass(Reg);
    TII.loadRegFromStackSlot(MBB, MI, Reg, CS.getFrameIdx(), RC, TRI, 0);
    assert(MI != MBB.begin() && "loadRegFromStackSlot didn't insert any code!");
  }

  return true;
}

bool RISCSimFrameLowering::hasFP(const MachineFunction &MF) const {
  const TargetRegisterInfo *RegInfo = MF.getSubtarget().getRegisterInfo();

  const MachineFrameInfo &MFI = MF.getFrameInfo();
  return MF.getTarget().Options.DisableFramePointerElim(MF) || // -fomit-frame-pointer
         RegInfo->hasStackRealignment(MF) || MFI.hasVarSizedObjects() ||
         MFI.isFrameAddressTaken();
  // Maybe just true? No: -fomit-frame-pointer
}

/// # From ARM

/// getFrameIndexReference - Provide a base+offset reference to an FI slot for
/// debug info.  It's the same as what we use for resolving the code-gen
/// references for now.  FIXME: This can go wrong when references are
/// SP-relative and simple call frames aren't used.
StackOffset RISCSimFrameLowering::getFrameIndexReference(const MachineFunction &MF, int FI,
                                             Register &FrameReg) const {

//  bool PreferFP = MF.getFunction().hasFnAttribute(Attribute::SanitizeHWAddress);
  const auto &MFI = MF.getFrameInfo();
//  int64_t ObjectOffset = MFI.getObjectOffset(FI);
//  bool isFixed = MFI.isFixedObjectIndex(FI);

  const auto *RegInfo = static_cast<const RISCSimRegisterInfo *>(MF.getSubtarget().getRegisterInfo());
//  const auto *AFI = MF.getInfo<RISCSimFunctionInfo>();
//  const auto &Subtarget = MF.getSubtarget<RISCSimSubtarget>();

// Remove FPOffset from use
//  int64_t FPOffset = getFPOffset(MF, ObjectOffset).getFixed();
//  int64_t Offset = getStackOffset(MF, ObjectOffset).getFixed();

  int Offset = MFI.getObjectOffset(FI) - getOffsetOfLocalArea() +
	           MFI.getOffsetAdjustment();

  int MinCSFI = 0;
  int MaxCSFI = -1;
  const auto &CSI = MFI.getCalleeSavedInfo();

  if (CSI.size()) {
    MinCSFI = CSI[0].getFrameIdx();
    MaxCSFI = CSI[CSI.size() - 1].getFrameIdx();
  }

  if (FI >= MinCSFI && FI <= MaxCSFI) {
    FrameReg = RISCSim::x6; // SP
    Offset += MFI.getStackSize();
  } else if (RegInfo->hasStackRealignment(MF) && !MFI.isFixedObjectIndex(FI)) {
    // Don't realize realigned stack - FPOffset don't count
    llvm_unreachable("");
    
//    assert(hasFP(MF) && "Re-aligned stack must have frame pointer");
//    UseFP = true;
  } else {
    // TODO: what's going on here
    FrameReg = RegInfo->getFrameRegister(MF);
    if (hasFP(MF)) {
	    llvm_unreachable("hasFP(MF) in getFrameIndexReference");
//      Offset += UFI->getVarArgsSaveSize(); // No varargs in curr implementation
    } else {
      Offset += MFI.getStackSize();
    }
  }

  return StackOffset::getFixed(Offset);
/*
  bool isCSR = !isFixed && ObjectOffset >= -((int)AFI->getCalleeSavedStackSize(MFI));

  // Use frame pointer to reference fixed objects. Use it for locals if
  // there are VLAs or a dynamically realigned SP (and thus the SP isn't
  // reliable as a base). Make sure useFPForScavengingIndex() does the
  // right thing for the emergency spill slot.
  bool UseFP = false;
  if (AFI->hasStackFrame()) {

    // Note: Keeping the following as multiple 'if' statements rather than
    // merging to a single expression for readability.
    //
    // Argument access should always use the FP.
    if (isFixed) {
      UseFP = hasFP(MF);
    } else if (isCSR && RegInfo->hasStackRealignment(MF)) {
      // # If have dynamically-sized alignment padding (more than 16 bytes)

      // References to the CSR area must use FP if we're re-aligning the stack
      // since the dynamically-sized alignment padding is between the SP/BP and
      // the CSR area.
      assert(hasFP(MF) && "Re-aligned stack must have frame pointer");
      UseFP = true;
    } else if (hasFP(MF) && !RegInfo->hasStackRealignment(MF)) {
      // # Common case

      // If the FPOffset is negative and we're producing a signed immediate, we
      // have to keep in mind that the available offset range for negative
      // offsets is smaller than for positive ones. If an offset is available
      // via the FP and the SP, use whichever is closest.
      bool FPOffsetFits = FPOffset >= -256;

      if (MFI.hasVarSizedObjects()) {
      	// # case when we have variable sized objects

        // If we have variable sized objects, we can use either FP or BP, as the
        // SP offset is unknown. We can use the base pointer if we have one and
        // FP is not preferred. If not, we're stuck with using FP.
        bool CanUseBP = RegInfo->hasBasePointer(MF);
        if (FPOffsetFits && CanUseBP) // Both are ok. Pick the best.
          UseFP = PreferFP;
        else if (!CanUseBP) // Can't use BP. Forced to use FP.
          UseFP = true;
        // else we can use BP and FP, but the offset from FP won't fit.
        // That will make us scavenge registers which we can probably avoid by
        // using BP. If it won't fit for BP either, we'll scavenge anyway.
      } else if (FPOffset >= 0) {
        // Use SP or FP, whichever gives us the best chance of the offset
        // being in range for direct access. If the FPOffset is positive,
        // that'll always be best, as the SP will be even further away.
        UseFP = true;
      } else if (MF.hasEHFunclets() && !RegInfo->hasBasePointer(MF)) {
      	// # This is Win64 case, maybe support only Linux-based?

        // Funclets access the locals contained in the parent's stack frame
        // via the frame pointer, so we have to use the FP in the parent
        // function.
        (void) Subtarget;
        assert(
            Subtarget.isCallingConvWin64(MF.getFunction().getCallingConv()) &&
            "Funclets should only be present on Win64");
        UseFP = true;
      } else {
        // We have the choice between FP and (SP or BP).
        if (FPOffsetFits && PreferFP) // If FP is the best fit, use it.
          UseFP = true;
      }
    }
  }

  // # Default stack alignment - 16 bytes, but maybe different and ARM support these
  assert(
      ((isFixed || isCSR) || !RegInfo->hasStackRealignment(MF) || !UseFP) &&
      "In the presence of dynamic stack pointer realignment, "
      "non-argument/CSR objects cannot be accessed through the frame pointer");

  if (UseFP) {
    FrameReg = RegInfo->getFrameRegister(MF);
    return StackOffset::getFixed(FPOffset);
  }

  // Use the base pointer if we have one.
  if (RegInfo->hasBasePointer(MF))
    FrameReg = RegInfo->getBaseRegister();
  else {
    assert(!MFI.hasVarSizedObjects() &&
           "Can't use SP when we have var sized objects.");
    FrameReg = RISCSim::x6; // SP
    // If we're using the red zone for this function, the SP won't actually
    // be adjusted, so the offsets will be negative. They're also all
    // within range of the signed 9-bit immediate instructions.
    if (canUseRedZone(MF))
      Offset -= AFI->getLocalStackSize();
  }

  return StackOffset::getFixed(Offset);*/
}

// Not preserve stack space within prologue for outgoing variables when the
// function contains variable size objects or there are vector objects accessed
// by the frame pointer.
// Let eliminateCallFramePseudoInstr preserve stack space for it.
bool RISCSimFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
  return !MF.getFrameInfo().hasVarSizedObjects();
}