//===- RISCSimInstrInfo.td - Target Description for RISCSim -*- tablegen -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes the RISCSim instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

include "RISCSimInstrFormats.td"

class SImmAsmOperand<int width> : AsmOperandClass {
  let Name = "SImm" # width;
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

def simm14 : Operand<i32>, ImmLeaf<i32, [{return isInt<14>(Imm);}]> {
  let ParserMatchClass = SImmAsmOperand<14>;
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeSImmOperand<14>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isInt<14>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  let OperandType = "OPERAND_SIMM14";
//let OperandNamespace = "RISCSimOp";
}

// cmake --build build
// export CLANG=/home/id74/concurrency/LLVM-project/llvm-project/build/bin/clang
// ./build/bin/clang -print-targets
// ${CLANG} -print-targets

//===----------------------------------------------------------------------===//
// Arithmetic & Logical instructions
//===----------------------------------------------------------------------===//

class Arith3RegInst<bits<8> opcode, string instrAsm, SDPatternOperator opNode, bit isComm>
  : Instr3RFormat<opcode, (outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
            instrAsm#"\t$rd, $rs1, $rs2",
            [(set GPR:$rd, (opNode GPR:$rs1, GPR:$rs2))]> {
  let isCommutable = isComm;
  let isReMaterializable = 0;
}

def ADD : Arith3RegInst<0x08, "add", add, 1>;
def SUB : Arith3RegInst<0x09, "sub", sub, 0>;
def AND : Arith3RegInst<0x0A, "and", and, 1>;
def OR  : Arith3RegInst<0x0B, "or" , or , 1>;
def XOR : Arith3RegInst<0x0C, "xor", xor, 1>;

class Arith2RegImmInst<bits<8> opcode, string instrAsm, SDPatternOperator opNode, bit isComm>
  : Instr2R1IFormat<opcode, (outs GPR:$rd), (ins GPR:$rs1, simm14:$imm),
            instrAsm#"\t$rd, $rs1, $imm",
            [(set GPR:$rd, (opNode GPR:$rs1, GPR:$imm))]> {
  let isCommutable = isComm;
  let isReMaterializable = 0;
}

def ADDI : Arith3RegInst<0x18, "add", add, 1>;
def SUBI : Arith3RegInst<0x19, "sub", sub, 0>;
def ANDI : Arith3RegInst<0x1A, "and", and, 1>;
def ORI  : Arith3RegInst<0x1B, "or" , or , 1>;
def XORI : Arith3RegInst<0x1C, "xor", xor, 1>;

//===----------------------------------------------------------------------===//
// Shift instructions
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Move instructions
//===----------------------------------------------------------------------===//
/*
// movli
let isAsCheapAsAMove = 1, isCodeGenOnly = 1, isReMaterializable = 1 in
def MOVLI : SimInst<0x0, (outs GPR:$rd), (ins simm16:$imm),
                     "MOVli $rd $imm",
                     [(set GPR:$rd, simm16:$imm)]>;
*/
//===----------------------------------------------------------------------===//
// Load and store instructions
//===----------------------------------------------------------------------===//

def ImmAddrOffset : Operand<i32>, ImmLeaf<i32, 
        [{ return Imm >= 0 && Imm < 16383 && (Imm & 0x3 == 0); }]> {

  let PrintMethod = "printImmAddrOffset"; // Custom
  let ParserMatchClass = SImmAsmOperand<14>; // ?? !cast<AsmOperandClass>(ImmAddrOffset);
}
def mem32 : Operand<i32> {
  let MIOperandInfo = (ops GPR, ImmAddrOffset);
//let DecoderMethod = "decodeMem32Operand"; // Custom; disassembler
  let EncoderMethod = "getMemRegEncoding"; // Custom; code emitter
  let OperandType = "OPERAND_MEMORY";
  let PrintMethod = "printMemOperand";
}
def AddrShift4 : ComplexPattern<iPTR, 2, "selectMemRegAddrISH4", [frameindex]>;

// Load instructions
let mayLoad = 1 in {

  class LoadInstr<bits<8> opcode>
    : Instr2R1IFormat<opcode, (outs GPR:$rd), (ins mem32:$addr), "load\t$rd, $addr",
               [(set GPR:$rd, (load AddrShift4:$addr))]> {

    bits<14> addr;
    let imm{13-0} = addr{13-0};
  }
}
def LOAD : LoadInstr<0x28>;

// Store instructions
let mayStore = 1 in {

  class StoreInstr<bits<8> opcode>
    : Instr2R1IFormat<opcode, (outs), (ins GPR:$rd, mem32:$addr), "store\t$rd, $addr",
               [(store GPR:$rd, AddrShift4:$addr)]> {

    bits<14> addr;
    let imm{13-0} = addr{13-0};
  }
}
def STORE : LoadInstr<0x29>;

//===----------------------------------------------------------------------===//
// Conditional branch instructions
//===----------------------------------------------------------------------===//

// What is bb: ?

def RISCSimPCRelTargetAsmOperand : AsmOperandClass {
  let Name = "PCRelTarget"; // Custom?
  let ParserMethod = "parsePCRelTarget"; // Custom; asm parser
  let PredicateMethod = "isImm";
  let RenderMethod = "addImmOperands";
}

def brtarget : Operand<OtherVT> {
  let PrintMethod = "printBranchTarget"; // Custom; printer
  let EncoderMethod = "getBranchTargetEncoding"; // Custom; code emitter
//let DecoderMethod = "decodeBranchOperand"; // Custom; disassembler
  let ParserMatchClass = RISCSimPCRelTargetAsmOperand;
}

let isBranch = 1, isTerminator = 1 in {
  class CondBranchInstr<bits<8> opcode, string instrAsm, CondCode CC>
      : Instr2R1IFormat<opcode, (outs),
                 (ins GPR:$rd, GPR:$rs1, brtarget:$target),
                  instrAsm#"\t$rd, $rs1, $target",
                 [(brcc CC, GPR:$rd, GPR:$rs1,  bb:$target)]> {

    bits<14> target;
    let imm{13-0} = target{13-0};
  }
}

def BEQ   : CondBranchInstr<0x38, "beq", SETEQ>;
def BNE   : CondBranchInstr<0x39, "bne", SETNE>;
def BGE   : CondBranchInstr<0x3A, "bge", SETGE>;
def BLT   : CondBranchInstr<0x3B, "blt", SETLT>;
def BGEU  : CondBranchInstr<0x3C, "bgeu", SETUGE>;
def BLTU  : CondBranchInstr<0x3D, "bltu", SETULT>;

//===----------------------------------------------------------------------===//
// Call and jump instructions
//===----------------------------------------------------------------------===//

def  pcrel32call : Operand<iPTR> {
  let PrintMethod = "printCallOperand"; // Custom; asm printer
  let EncoderMethod = "getCallEncoding"; // Custom; code emitter
//let DecoderMethod = "decodeCallOperand"; // Custom; disassembler
  let ParserMatchClass = RISCSimPCRelTargetAsmOperand;
}

def jumptarget : Operand<OtherVT> {
  let PrintMethod = "printJumpTarget"; // Custom
  let EncoderMethod = "getJumpTargetEncoding"; // Custom
//let DecoderMethod = "decodeJumpOperand"; // Custom
  let ParserMatchClass = RISCSimPCRelTargetAsmOperand;
}

/*
def JMP : DirectJmpInst<0x48, (outs), (ins jumptarget:$offset), "j\t$offset", [(br bb:$offset)]> {

  bits<14> offset;
  let imm{23-10} = offset{13-0};

  let isBranch = 1;  
  let isTerminator = 1;
  let isBarrier = 1;
}
*/

def JMPX : IndirectJmpInst<0x49, (outs), (ins GPR:$rd), "jx\t$rd", [(brind GPR:$rd)]> {
  let isBranch = 1;  
  let isTerminator = 1;
  let isBarrier = 1;
  let isIndirectBranch = 1;
}

def CALL : IndirectJmpInst<0x4A, (outs), (ins GPR:$rd), "call\t$rd", [(brind GPR:$rd)]> {

  let isCall = 1;
  let Defs = []; // No non-operand registers to modify?
}

def RET : EmptyInst<0x4B, (outs), (ins), "ret", []> {

  let isReturn = 1;
  let isTerminator = 1;
  let isBarrier = 1;
  let Uses = []; // No non-operand registers to use?
}
