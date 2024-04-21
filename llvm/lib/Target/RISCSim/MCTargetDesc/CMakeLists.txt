add_llvm_component_library(LLVMRISCSimDesc
  RISCSimMCTargetDesc.cpp
  RISCSimMCAsmInfo.cpp
  RISCSimInstPrinter.cpp

  LINK_COMPONENTS
  MC
  Support
  RISCSimInfo

  ADD_TO_COMPONENT
  RISCSim
 )