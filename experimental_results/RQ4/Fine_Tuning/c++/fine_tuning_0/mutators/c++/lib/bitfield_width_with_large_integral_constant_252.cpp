//source file
#include "../include/Bitfield_Width_with_Large_Integral_Constant_252.h"

// ========================================================================================================
#define MUT252_OUTPUT 1

void MutatorFrontendAction_252::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BF = Result.Nodes.getNodeAs<clang::FieldDecl>("bitfield")) {
      if (!BF || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BF->getLocation()))
        return;
      if (!BF->isBitField())
        return;

      auto widthExpr = BF->getBitWidth();
      if (!widthExpr)
        return;

      auto widthStr = stringutils::rangetoStr(*(Result.SourceManager),
                                              widthExpr->getSourceRange());

      std::string largeWidthStr = "1 + (unsigned __int128)0xffffffffffffffff";
      llvm::outs() << "Original width: " << widthStr << ", Mutated width: "
                   << largeWidthStr << "\n";

      Rewrite.ReplaceText(widthExpr->getSourceRange(), largeWidthStr);
    }
}
  
void MutatorFrontendAction_252::MutatorASTConsumer_252::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl().bind("bitfield");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}