//source file
#include "../include/constexpr_incorrect_definition_318.h"

// ========================================================================================================
#define MUT318_OUTPUT 1

void MutatorFrontendAction_318::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("ConstexprFunc")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isConstexpr())
        return;
      if (!FD->hasBody())
        return;
      auto definition = stringutils::rangetoStr(*(Result.SourceManager),
                                                FD->getSourceRange());
      definition += "=delete";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()),
                          definition);
    }
}
  
void MutatorFrontendAction_318::MutatorASTConsumer_318::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("ConstexprFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}