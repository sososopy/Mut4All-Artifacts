//source file
#include "../include/incomplete_static_assert_13.h"

// ========================================================================================================
#define MUT13_OUTPUT 1

void MutatorFrontendAction_13::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
        if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(SA->getLocation()))
            return;

        // Get the source code text of target node
        auto assertionText = stringutils::rangetoStr(*(Result.SourceManager), SA->getSourceRange());

        // Perform mutation on the source code text by removing part of the message
        size_t pos = assertionText.find(",");
        if (pos != std::string::npos) {
            assertionText = assertionText.substr(0, pos) + ", \"This assertion message is incomplete ";
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(SA->getSourceRange()), assertionText);
    }
    else if (auto *BE = Result.Nodes.getNodeAs<clang::Expr>("BooleanExpr")) {
        if (!BE || !Result.Context->getSourceManager().isWrittenInMainFile(BE->getExprLoc()))
            return;

        // Get the source code text of target node
        auto exprText = stringutils::rangetoStr(*(Result.SourceManager), BE->getSourceRange());
        
        // Perform mutation by introducing a new incomplete static_assert
        std::string newStaticAssert = "static_assert(" + exprText + ", \"This assertion message is incomplete ";
        
        // Insert the new static_assert before the boolean expression
        Rewrite.InsertTextBefore(BE->getExprLoc(), newStaticAssert);
    }
}
  
void MutatorFrontendAction_13::MutatorASTConsumer_13::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher staticAssertMatcher = staticAssertDecl().bind("StaticAssert");
    StatementMatcher booleanExprMatcher = expr(hasType(booleanType())).bind("BooleanExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(staticAssertMatcher, &callback);
    matchFinder.addMatcher(booleanExprMatcher, &callback);
    matchFinder.matchAST(Context);
}