#ifndef LLVM_LIB_TARGET_RISCSIM_RISCSIMREGISTERINFO_H
#define LLVM_LIB_TARGET_RISCSIM_RISCSIMREGISTERINFO_H

#define GET_REGINFO_HEADER
#include "RISCSimGenRegisterInfo.inc"

namespace llvm {

struct RISCSimRegisterInfo : public RISCSimGenRegisterInfo {
public:
  RISCSimRegisterInfo();
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_RISCSIM_RISCSIMREGISTERINFO_H
