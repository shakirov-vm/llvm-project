#ifndef LLVM_LIB_TARGET_RISCSim_RISCSim_H
#define LLVM_LIB_TARGET_RISCSim_RISCSim_H

#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"

#include "MCTargetDesc/RISCSimMCTargetDesc.h"
#define RISCSIM_DUMP(Color)                                                    \
  {                                                                            \
    llvm::errs().changeColor(Color)                                            \
        << __func__ << "\n\t\t" << __FILE__ << ":" << __LINE__ << "\n";        \
    llvm::errs().changeColor(llvm::raw_ostream::WHITE);                        \
  }

#define RISCSIM_DUMP_RED RISCSIM_DUMP(llvm::raw_ostream::RED)
#define RISCSIM_DUMP_GREEN RISCSIM_DUMP(llvm::raw_ostream::GREEN)
#define RISCSIM_DUMP_YELLOW RISCSIM_DUMP(llvm::raw_ostream::YELLOW)
#define RISCSIM_DUMP_CYAN RISCSIM_DUMP(llvm::raw_ostream::CYAN)
#define RISCSIM_DUMP_MAGENTA RISCSIM_DUMP(llvm::raw_ostream::MAGENTA)
#define RISCSIM_DUMP_WHITE RISCSIM_DUMP(llvm::raw_ostream::WHITE)

namespace llvm {
class RISCSimTargetMachine;
class FunctionPass;
class RISCSimSubtarget;
class AsmPrinter;
class InstructionSelector;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineOperand;
class PassRegistry;

bool lowerRISCSimMachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI,
                                    AsmPrinter &AP);
bool LowerRISCSimMachineOperandToMCOperand(const MachineOperand &MO,
                                         MCOperand &MCOp, const AsmPrinter &AP);

FunctionPass *createRISCSimISelDag(RISCSimTargetMachine &TM);

} //

#endif // LLVM_LIB_TARGET_RISCSim_RISCSim_H
