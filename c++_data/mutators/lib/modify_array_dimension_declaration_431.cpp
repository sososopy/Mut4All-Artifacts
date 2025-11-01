//source file
#include "../include/Modify_Array_Dimension_Declaration_431.h"

// ========================================================================================================
#define MUT431_OUTPUT 1

void MutatorFrontendAction_431::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (auto *AT = VD->getType()->getAsArrayTypeUnsafe()) {
        if (auto *CAT = dyn_cast<ConstantArrayType>(AT)) {
          llvm::APInt size = CAT->getSize();
          std::string newExpr = (llvm::Twine(size.getZExtValue()) + " + 5 * 0").str();
          Rewrite.ReplaceText(CAT->getSizeExpr()->getSourceRange(), newExpr);
        }
      }
    }
}
  
void MutatorFrontendAction_431::MutatorASTConsumer_431::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(arrayType())).bind("arrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}