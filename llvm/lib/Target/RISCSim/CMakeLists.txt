
add_llvm_component_group(RISCSim)

add_llvm_target(RISCSimCodeGen
  RISCSimTargetMachine.cpp

  LINK_COMPONENTS
  CodeGen
  Core
  Support
  Target
  RISCSimInfo

  ADD_TO_COMPONENT
  RISCSim
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)