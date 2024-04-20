#ifndef LLVM_LIB_TARGET_RISCSIM_MCTARGETDESC_RISCSIMINFO_H
#define LLVM_LIB_TARGET_RISCSIM_MCTARGETDESC_RISCSIMINFO_H

#include "llvm/MC/MCInstrDesc.h"

namespace llvm {

namespace SimOp {
enum OperandType : unsigned {
  OPERAND_SIMM16 = MCOI::OPERAND_FIRST_TARGET,
};
} // namespace SimOp

} // end namespace llvm

#endif