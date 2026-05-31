//source file
#include "../include/Misplaced_Deduction_Guide_35.h"

// ========================================================================================================
#define MUT35_OUTPUT 1

void MutatorFrontendAction_35::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      auto className = TD->getNameAsString();
      std::string deductionGuide = "\n/*mut35*/" + className + "() -> " + className + "<int>;";
      SourceLocation insertLoc = TD->getEndLoc();
      Rewrite.InsertTextAfterToken(insertLoc, deductionGuide);
    }
}
  
void MutatorFrontendAction_35::MutatorASTConsumer_35::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}