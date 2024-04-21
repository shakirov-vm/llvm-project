#ifndef LLVM_LIB_TARGET_RISCSIM_RISCSIMTARGETSTREAMER_H
#define LLVM_LIB_TARGET_RISCSIM_RISCSIMTARGETSTREAMER_H

#include "llvm/MC/MCStreamer.h"

namespace llvm {

class RISCSimTargetStreamer : public MCTargetStreamer {
public:
  RISCSimTargetStreamer(MCStreamer &S);
  ~RISCSimTargetStreamer() override;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_RISCSIM_RISCSIMTARGETSTREAMER_H