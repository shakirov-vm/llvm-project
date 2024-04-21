
add_llvm_component_group(RISCSim)

set(LLVM_TARGET_DEFINITIONS RISCSim.td)

tablegen(LLVM RISCSimGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM RISCSimGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM RISCSimGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM RISCSimGenCallingConv.inc -gen-callingconv)
tablegen(LLVM RISCSimGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM RISCSimGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM RISCSimGenDAGISel.inc -gen-dag-isel)

add_public_tablegen_target(RISCSimCommonTableGen)

add_llvm_target(RISCSimCodeGen
  RISCSimTargetMachine.cpp
  RISCSimRegisterInfo.cpp
  RISCSimInstrInfo.cpp
  RISCSimSubtarget.cpp
  RISCSimAsmPrinter.cpp
  RISCSimFrameLowering.cpp
  RISCSimISelDagToDag.cpp
  RISCSimISelLowering.cpp
  RISCSimMCInstLower.cpp

  LINK_COMPONENTS
  AsmPrinter
  CodeGen
  Core
  MC
  RISCSimDesc
  SelectionDAG
  Support
  Target
  RISCSimInfo

  ADD_TO_COMPONENT
  RISCSim
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)