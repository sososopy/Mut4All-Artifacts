//source file
#include "../include/Deduction_Guide_As_Member_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DG = Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>("deductionGuide")) {
        if (!DG || !Result.Context->getSourceManager().isWrittenInMainFile(DG->getLocation()))
            return;

        auto dgText = stringutils::rangetoStr(*Result.SourceManager, DG->getSourceRange());
        if (targetClasses.empty()) return;

        auto target = targetClasses[0];
        SourceLocation endLoc = target->getEndLoc();
        Rewrite.InsertText(endLoc, dgText, true, true);
        Rewrite.ReplaceText(DG->getSourceRange(), "");
    } else if (auto *RC = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("targetClass")) {
        if (!RC || !Result.Context->getSourceManager().isWrittenInMainFile(RC->getLocation()))
            return;
        targetClasses.push_back(RC);
    }
}
  
void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher1 = cxxDeductionGuideDecl().bind("deductionGuide");
    DeclarationMatcher matcher2 = cxxRecordDecl().bind("targetClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}