#include "RISCSimMCAsmInfo.h"
#include "RISCSim.h"

using namespace llvm;

RISCSimELFMCAsmInfo::RISCSimELFMCAsmInfo(const Triple &TT) {
  RISCSIM_DUMP_MAGENTA
  SupportsDebugInformation = false;
  Data16bitsDirective = "\t.short\t";
  Data32bitsDirective = "\t.word\t";
  Data64bitsDirective = nullptr;
  ZeroDirective = "\t.space\t";
  CommentString = ";";

  UsesELFSectionDirectiveForBSS = false;
  AllowAtInName = true;
  HiddenVisibilityAttr = MCSA_Invalid;
  HiddenDeclarationVisibilityAttr = MCSA_Invalid;
  ProtectedVisibilityAttr = MCSA_Invalid;

  ExceptionsType = ExceptionHandling::None;
} 
