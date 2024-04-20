//===- RISCSimInstrFormats.td - RISCSim Instruction Formats --*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Base class for RISCSim 32 bit Formats
class RISCSimInst<dag outs, dag ins, string asmstr, list<dag> pattern>
  : Instruction {

  let Namespace = "RISCSim";

  let Size = 4;

  let OutOperandList = outs;
  let InOperandList  = ins;

  let AsmString   = asmstr;
  let Pattern     = pattern;

  field bits<32> Inst;
  field bits<32> SoftFail = 0;
}

class Instr3RFormat<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>
  : RISCSimInst<outs, ins, asmstr, pattern> {

  bits<5> rd;
  bits<5> rs1;
  bits<5> rs2;  

  let Inst{31-24} = op;
  let Inst{23-19} = rd;
  let Inst{18-14} = rs1;
  let Inst{13-9} = rs2;
  let Inst{8-0} = 0; // reserved
}

// And this also branch
class Instr2R1IFormat<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>
  : RISCSimInst<outs, ins, asmstr, pattern> {

  bits<5> rd;
  bits<5> rs1;
  bits<14> imm;

  let Inst{31-24} = op;
  let Inst{23-19} = rd;
  let Inst{18-14} = rs1;
  let Inst{13-0} = imm;
}

class DirectJmpInst<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>
  : RISCSimInst<outs, ins, asmstr, pattern> {

  bits<14> imm;

  let Inst{31-24} = op;
  let Inst{23-10} = imm;
  let Inst{9-0} = 0x0;
}

class IndirectJmpInst<bits<8> op,dag outs, dag ins, string asmstr, list<dag> pattern>
  : RISCSimInst<outs, ins, asmstr, pattern> {

  bits<5> rd;

  let Inst{31-24} = op;
  let Inst{23-19} = rd;
  let Inst{18-0} = 0x0;
}
// Graphical and ret
class EmptyInst<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>
  : RISCSimInst<outs, ins, asmstr, pattern> {

  let Inst{31-24} = op;
  let Inst{23-0} = 0x0;
}
