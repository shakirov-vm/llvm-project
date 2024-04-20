#include "RISCSimInstrInfo.h"
#include "RISCSim.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "RISCSimGenInstrInfo.inc"

#define DEBUG_TYPE "RISCSim-inst-info"

RISCSimInstrInfo::RISCSimInstrInfo() : RISCSimGenInstrInfo() { 
	RISCSIM_DUMP_GREEN 
}
