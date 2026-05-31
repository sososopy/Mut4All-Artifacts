//source file
#include "../include/Deduction_Guide_Invalid_Syntax_Insertion_280.h"

// ========================================================================================================
#define MUT280_OUTPUT 1

void MutatorFrontendAction_280::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DG = Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>("deductionGuide")) {
        if (!DG || !Result.Context->getSourceManager().isWrittenInMainFile(DG->getLocation()))
            return;
        
        auto sourceCode = stringutils::rangetoStr(*Result.SourceManager, DG->getSourceRange());
        size_t arrowPos = sourceCode.find("->");
        if (arrowPos == std::string::npos) return;
        
        std::string className = DG->getDeclName().getAsString();
        std::string mutatedCode = sourceCode;
        mutatedCode.insert(arrowPos, " " + className);
        
        Rewrite.ReplaceText(DG->getSourceRange(), mutatedCode);
    }
}

void MutatorFrontendAction_280::MutatorASTConsumer_280::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxDeductionGuideDecl().bind("deductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}