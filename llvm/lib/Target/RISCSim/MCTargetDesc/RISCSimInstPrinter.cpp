#include "RISCSimInstPrinter.h"
#include "MCTargetDesc/RISCSimInfo.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#include "RISCSimGenAsmWriter.inc"

void RISCSimInstPrinter::printRegName(raw_ostream &O, MCRegister Reg) const {
  O << getRegisterName(Reg);
}

void RISCSimInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                  StringRef Annot, const MCSubtargetInfo &STI,
                                  raw_ostream &O) {
  printInstruction(MI, Address, O);
  printAnnotation(O, Annot);
}

static void printExpr(const MCExpr *Expr, raw_ostream &OS) {
  int Offset = 0;
  const MCSymbolRefExpr *SRE;

  if (!(SRE = cast<MCSymbolRefExpr>(Expr)))
    assert(false && "Unexpected MCExpr type.");
  // Don't need TODO?
  MCSymbolRefExpr::VariantKind Kind = SRE->getKind();

  switch (Kind) {
  case MCSymbolRefExpr::VK_None:
    break;
  default:
    report_fatal_error("Invalid kind!");
  }

  OS << SRE->getSymbol();

  if (Offset) {
    if (Offset > 0)
      OS << '+';
    OS << Offset;
  }

  if (Kind != MCSymbolRefExpr::VK_None)
    OS << ')';
}

void RISCSimInstPrinter::printOperand(const MCOperand &MC, raw_ostream &O) {
  if (MC.isReg())
    O << getRegisterName(MC.getReg());
  else if (MC.isImm())
    O << MC.getImm();
  else if (MC.isExpr())
    printExpr(MC.getExpr(), O);
  else
    report_fatal_error("Invalid operand");
}

void RISCSimInstPrinter::printOperand(const MCInst *MI, int OpNum,
                                     raw_ostream &O) {
  printOperand(MI->getOperand(OpNum), O);
}

void RISCSimInstPrinter::printMemOperand(const MCInst *MI, int OpNum,
                                        raw_ostream &OS) {
  OS << getRegisterName(MI->getOperand(OpNum).getReg());
  OS << ", ";
  printOperand(MI, OpNum + 1, OS);
}

void RISCSimInstPrinter::printBranchTarget(const MCInst *MI, int OpNum,
                                          raw_ostream &OS) {
const MCOperand &MC = MI->getOperand(OpNum);
  if (MI->getOperand(OpNum).isImm()) {
    int64_t Val = MC.getImm() + 4;
    OS << ". ";
    if (Val > 0)
      OS << '+';
    OS << Val;
  } else if (MC.isExpr())
    MC.getExpr()->print(OS, &MAI, true);
  else
    llvm_unreachable("Invalid operand");
}

void RISCSimInstPrinter::printJumpTarget(const MCInst *MI, int OpNum,
                                        raw_ostream &OS) {
  const MCOperand &MC = MI->getOperand(OpNum);
  if (MC.isImm()) {
    int64_t Val = MC.getImm() + 4;
    OS << ". ";
    if (Val > 0)
      OS << '+';
    OS << Val;
  } else if (MC.isExpr())
    MC.getExpr()->print(OS, &MAI, true);
  else
    llvm_unreachable("Invalid operand");
  ;
}

void RISCSimInstPrinter::printCallOperand(const MCInst *MI, int OpNum,
                                         raw_ostream &OS) {
  const MCOperand &MC = MI->getOperand(OpNum);
  if (MC.isImm()) {
    int64_t Val = MC.getImm() + 4;
    OS << ". ";
    if (Val > 0)
      OS << '+';
    OS << Val;
  } else if (MC.isExpr())
    MC.getExpr()->print(OS, &MAI, true);
  else
    llvm_unreachable("Invalid operand");
}

void RISCSimInstPrinter::printImmAddrOffset(const MCInst *MI, int OpNum,
                                                   raw_ostream &O) {
  if (MI->getOperand(OpNum).isImm()) {
    int64_t Value = MI->getOperand(OpNum).getImm();
    assert(
        (Value >= 0 && Value < 16383 && ((Value & 0x3) == 0)) &&
        "Invalid argument, value must be multiples of four in range [0, 16383]");
    O << Value;
  } else
    printOperand(MI, OpNum, O);
}
