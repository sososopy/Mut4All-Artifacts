//source file
#include "../include/Modify_Template_Constraint_74.h"

// ========================================================================================================
#define MUT74_OUTPUT 1

void MutatorFrontendAction_74::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpec")) {
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getLocation()))
        return;

      auto constraints = TS->getTemplateArgs().asArray();
      if (constraints.size() == 0)
        return;

      std::string mutatedConstraint = "(sizeof(T) + 2 > 6)";
      SourceLocation startLoc = TS->getBeginLoc();
      SourceLocation endLoc = TS->getEndLoc().getLocWithOffset(1);
      Rewrite.ReplaceText(SourceRange(startLoc, endLoc), mutatedConstraint);
    }
}
  
void MutatorFrontendAction_74::MutatorASTConsumer_74::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}