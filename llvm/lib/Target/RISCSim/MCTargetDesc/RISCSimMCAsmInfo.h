#ifndef LLVM_LIB_TARGET_RISCSIM_MCTARGETDESC_RISCSIMMCASMINFO_H
#define LLVM_LIB_TARGET_RISCSIM_MCTARGETDESC_RISCSIMMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {

class Triple;

class RISCSimELFMCAsmInfo : public MCAsmInfoELF {
public:
  explicit RISCSimELFMCAsmInfo(const Triple &TheTriple);
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_RISCSIM_MCTARGETDESC_RISCSIMMCASMINFO_H
