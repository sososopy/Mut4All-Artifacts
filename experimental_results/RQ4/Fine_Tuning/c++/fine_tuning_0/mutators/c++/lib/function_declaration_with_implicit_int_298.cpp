//source file
#include "../include/Function_Declaration_with_Implicit_Int_298.h"

// ========================================================================================================
#define MUT298_OUTPUT 1

void MutatorFrontendAction_298::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->getReturnType().getAsString() == "auto")
        return;
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      if (MT->getReturnType().getAsString() == "void") {
        return;
      }
      auto returntype = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getReturnTypeSourceRange());
      llvm::outs() << returntype << '\n';
      llvm::outs() << declaration << '\n';
      if (returntype != "") {
        declaration.erase(declaration.find(returntype), returntype.size());
      }
      llvm::outs() << declaration << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_298::MutatorASTConsumer_298::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}