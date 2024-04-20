// Register file declaration
class RISCSimReg<bits<16> Enc, string n> : Register<n>{
    let HWEncoding = Enc;
    let Namespace = "RISCSim";
}

// Registers
foreach I = 0-31 in {
    def x#I : RISCSimReg<I, "x"#I>;
}

// Register class with allocation order
def GPR : RegisterClass<"RISCSim", [i32], 32, (add
  x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15,
  x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31)>;
