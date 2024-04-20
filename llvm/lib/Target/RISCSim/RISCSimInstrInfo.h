#ifndef LLVM_LIB_TARGET_RISCSIM_RISCSIMINSTRINFO_H
#define LLVM_LIB_TARGET_RISCSIM_RISCSIMINSTRINFO_H

#include "RISCSimRegisterInfo.h"
#include "MCTargetDesc/RISCSimInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "RISCSimGenInstrInfo.inc"

namespace llvm {

class RISCSimSubtarget;

class RISCSimInstrInfo : public RISCSimGenInstrInfo {
public:
  RISCSimInstrInfo();

};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_RISCSIM_RISCSIMINSTRINFO_H
