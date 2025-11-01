//source file
#include "../include/malformed_destructor_syntax_378.h"

// ========================================================================================================
#define MUT378_OUTPUT 1

void MutatorFrontendAction_378::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DT = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Destructor")) {
      if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getLocation()))
        return;

      if (DT->hasBody()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   DT->getSourceRange());
        declaration = "~" + DT->getParent()->getNameAsString() + "() = {};";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(DT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_378::MutatorASTConsumer_378::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxDestructorDecl().bind("Destructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}