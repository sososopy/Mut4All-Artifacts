//source file
#include "../include/Remove_Template_Class_Deduction_Guide_494.h"

// ========================================================================================================
#define MUT494_OUTPUT 1

void MutatorFrontendAction_494::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>("DeductionGuide")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto deduction_guide = stringutils::rangetoStr(*(Result.SourceManager),
                                                     MT->getSourceRange());
      llvm::outs() << deduction_guide << '\n';
      deduction_guide = "/*mut494*/" + deduction_guide;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), deduction_guide);
    }
}
  
void MutatorFrontendAction_494::MutatorASTConsumer_494::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxDeductionGuideDecl().bind("DeductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}