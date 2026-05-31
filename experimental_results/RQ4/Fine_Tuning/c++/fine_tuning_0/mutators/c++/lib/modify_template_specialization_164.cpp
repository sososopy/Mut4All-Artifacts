//source file
#include "../include/Modify_Template_Specialization_164.h"

// ========================================================================================================
#define MUT164_OUTPUT 1

void MutatorFrontendAction_164::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << declaration << '\n';
      declaration.insert(declaration.find("<") + 1, "mut_164 ");
      declaration.insert(declaration.find(">"), " mut_164");
      llvm::outs() << declaration << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_164::MutatorASTConsumer_164::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}