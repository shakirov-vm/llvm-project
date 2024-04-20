#include "RISCSimRegisterInfo.h"
#include "RISCSim.h"
#include "RISCSimFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "RISCSimGenRegisterInfo.inc"

RISCSimRegisterInfo::RISCSimRegisterInfo() : RISCSimGenRegisterInfo(RISCSim::x0) {
  RISCSIM_DUMP_GREEN
}