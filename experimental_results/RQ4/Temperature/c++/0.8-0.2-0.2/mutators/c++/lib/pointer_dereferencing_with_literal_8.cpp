//source file
#include "../include/pointer_dereferencing_with_literal_8.h"

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *DerefExpr = Result.Nodes.getNodeAs<clang::UnaryOperator>("DerefExpr")) {
        // Filter nodes in header files
        if (!DerefExpr || !Result.Context->getSourceManager().isWrittenInMainFile(DerefExpr->getBeginLoc()))
            return;
        
        // Ensure it's a dereference operation
        if (DerefExpr->getOpcode() == clang::UO_Deref) {
            // Perform mutation by replacing the pointer operand with a literal
            auto DerefSourceRange = DerefExpr->getSourceRange();
            Rewrite.ReplaceText(DerefSourceRange, "*0");
        }
    }
}

void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    StatementMatcher matcher = unaryOperator(hasOperatorName("*"), hasUnaryOperand(ignoringParenImpCasts(declRefExpr(to(varDecl(hasType(pointerType()))))))).bind("DerefExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}