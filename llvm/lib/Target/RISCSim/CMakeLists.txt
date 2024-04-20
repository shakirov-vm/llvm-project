
add_llvm_component_group(RISCSim)

set(LLVM_TARGET_DEFINITIONS RISCSim.td)

tablegen(LLVM RISCSimGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM RISCSimGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM RISCSimGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(RISCSimCommonTableGen)

add_llvm_target(RISCSimCodeGen
  RISCSimTargetMachine.cpp
  RISCSimRegisterInfo.cpp
  RISCSimInstrInfo.cpp
  RISCSimSubtarget.cpp

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