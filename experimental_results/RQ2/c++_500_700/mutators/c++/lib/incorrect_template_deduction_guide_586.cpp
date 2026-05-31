//source file
#include "../include/incorrect_template_deduction_guide_586.h"

// ========================================================================================================
#define MUT586_OUTPUT 1

void MutatorFrontendAction_586::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;

      auto className = CTD->getNameAsString();
      std::string deductionGuide = className + "() " + className + " obj;";
      deductionGuide = "/*mut586*/" + deductionGuide;

      SourceLocation insertLoc = CTD->getEndLoc().getLocWithOffset(1);
      Rewrite.InsertText(insertLoc, deductionGuide, true, true);
    }
}
  
void MutatorFrontendAction_586::MutatorASTConsumer_586::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}