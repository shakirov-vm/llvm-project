//===----------------------------------------------------------------------===//
// This describes the calling conventions for RISCSim architecture.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// RISCSim Return Value Calling Convention
//===----------------------------------------------------------------------===//
def RetCC_RISCSim : CallingConv<[
  // i32 return values
  CCIfType<[i32, i64], CCAssignToReg<[x0, x1, x2, x3]>>,

  // Integer values get stored in stack slots that are 4 bytes in
  // size and 4-byte aligned.
  CCIfType<[i64], CCAssignToStack<8, 4>>,
  CCIfType<[i32], CCAssignToStack<4, 4>>
]>;

//===----------------------------------------------------------------------===//
// RISCSim Argument Calling Conventions
//===----------------------------------------------------------------------===//
def CC_RISCSim : CallingConv<[
  // Promote i8/i16 arguments to i32.
  CCIfType<[i8, i16], CCPromoteToType<i32>>,

  // The first 4 integer arguments are passed in integer registers.
  CCIfType<[i32, i64], CCAssignToReg<[x0, x1, x2, x3]>>,

  // Integer values get stored in stack slots that are 4 bytes in
  // size and 4-byte aligned.
  CCIfType<[i64], CCAssignToStack<8, 4>>,
  CCIfType<[i32], CCAssignToStack<4, 4>>
]>;
// CSR is NOT control-status regs
def CSR_RISCSim : CalleeSavedRegs<(add x8, x9, x10, x11, x12, x13, x14, x15,
  x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31)>;