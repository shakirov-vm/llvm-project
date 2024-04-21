#include "MCTargetDesc/RISCSimInstPrinter.h"
#include "TargetInfo/RISCSimTargetInfo.h"
#include "RISCSim.h"
#include "RISCSimSubtarget.h"
#include "RISCSimTargetMachine.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

// TODO not fixed yet

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

namespace {

class RISCSimAsmPrinter : public AsmPrinter {
  const MCSubtargetInfo *STI;

public:
  explicit RISCSimAsmPrinter(TargetMachine &TM,
                          std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)), STI(TM.getMCSubtargetInfo()) {}

  StringRef getPassName() const override { return "RISCSim Assembly Printer"; }

  bool emitPseudoExpansionLowering(MCStreamer &OutStreamer,
                                   const MachineInstr *MI);

  void emitInstruction(const MachineInstr *MI) override;

  // Used in pseudo lowerings
  bool lowerOperand(const MachineOperand &MO, MCOperand &MCOp) const {
    return LowerRISCSimMachineOperandToMCOperand(MO, MCOp, *this);
  }
};

} // end anonymous namespace

// TODO: Don't need pseudo-instructions?

// RISCSimple pseudo-instructions have their lowering (with expansion to real
// instructions) auto-generated.
#include "RISCSimGenMCPseudoLowering.inc"

void RISCSimAsmPrinter::emitInstruction(const MachineInstr *MI) {
  // Do any auto-generated pseudo lowerings.
  if (emitPseudoExpansionLowering(*OutStreamer, MI))
    return;

  MCInst TmpInst;
  if (!lowerRISCSimMachineInstrToMCInst(MI, TmpInst, *this))
    EmitToStreamer(*OutStreamer, TmpInst);
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRISCSimAsmPrinter() {
  RegisterAsmPrinter<RISCSimAsmPrinter> X(getTheRISCSimTarget());
}