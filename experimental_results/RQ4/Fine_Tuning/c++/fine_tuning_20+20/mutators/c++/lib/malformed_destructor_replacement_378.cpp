//source file
#include "../include/malformed_destructor_replacement_378.h"

// ========================================================================================================
#define MUT378_OUTPUT 1

void MutatorFrontendAction_378::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Dtor")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      content = "~" + content + "()={}";
      content = "/*mut378*/" + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_378::MutatorASTConsumer_378::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxDestructorDecl().bind("Dtor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}