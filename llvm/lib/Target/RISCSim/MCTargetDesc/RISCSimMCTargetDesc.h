#ifndef LLVM_LIB_TARGET_RISCSIM_MCTARGETDESC_RISCSIMMCTARGETDESC_H
#define LLVM_LIB_TARGET_RISCSIM_MCTARGETDESC_RISCSIMMCTARGETDESC_H

// Defines symbolic names for RISCSim registers.  This defines a mapping from
// register name to register number.
//
#define GET_REGINFO_ENUM
#include "RISCSimGenRegisterInfo.inc"

// Defines symbolic names for the RISCSim instructions.
#define GET_INSTRINFO_ENUM
#include "RISCSimGenInstrInfo.inc"

#endif // LLVM_LIB_TARGET_RISCSIM_MCTARGETDESC_RISCSIMMCTARGETDESC_H