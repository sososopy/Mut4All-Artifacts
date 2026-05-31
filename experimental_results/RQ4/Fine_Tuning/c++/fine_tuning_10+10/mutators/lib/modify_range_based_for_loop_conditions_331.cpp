//source file
#include "../include/Modify_Range_Based_For_Loop_Conditions_331.h"

// ========================================================================================================
#define MUT331_OUTPUT 1

void MutatorFrontendAction_331::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ForStmt = Result.Nodes.getNodeAs<clang::CXXForRangeStmt>("forRangeLoop")) {
        if (!ForStmt || !Result.Context->getSourceManager().isWrittenInMainFile(
                ForStmt->getBeginLoc()))
            return;

        auto *ArrayDecl = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl");
        if (!ArrayDecl)
            return;

        // Get the source code text of target node
        auto RangeInit = ForStmt->getRangeInit();
        std::string originalRange = stringutils::rangetoStr(*(Result.SourceManager), RangeInit->getSourceRange());

        // Construct a ternary expression
        std::string ternaryExpr = "(1 > 0 ? " + originalRange + " : newArr)";
        
        // Insert new array declaration if not already present
        if (candidateArrays.empty()) {
            std::string newArrayDecl = "double newArr[] = {1.1, 2.2, 3.3};\n";
            Rewrite.InsertTextBefore(ForStmt->getBeginLoc(), newArrayDecl);
            candidateArrays.push_back(ArrayDecl);
        }

        // Perform mutation on the source code text by applying string replacement
        Rewrite.ReplaceText(RangeInit->getSourceRange(), ternaryExpr);
    }
}
  
void MutatorFrontendAction_331::MutatorASTConsumer_331::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    auto matcher = cxxForRangeStmt(
        hasLoopVariable(anything()),
        hasRangeInit(declRefExpr(to(varDecl().bind("arrayDecl"))))
    ).bind("forRangeLoop");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}