//source file
#include "../include/modify_using_enum_with_bitwise_83.h"

// ========================================================================================================
#define MUT83_OUTPUT 1

void MutatorFrontendAction_83::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("bitwiseOp")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
            return;

        // Get the source code text of the target node
        auto lhs = BO->getLHS()->IgnoreParenImpCasts();
        auto rhs = BO->getRHS()->IgnoreParenImpCasts();

        std::string lhsText = Lexer::getSourceText(CharSourceRange::getTokenRange(lhs->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
        std::string rhsText = Lexer::getSourceText(CharSourceRange::getTokenRange(rhs->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();

        // Perform mutation on the source code text by applying string replacement
        std::string mutatedExpr = "static_cast<float>(" + rhsText + " | " + lhsText + ")";
        
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), mutatedExpr);
    }
}
  
void MutatorFrontendAction_83::MutatorASTConsumer_83::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    auto matcher = binaryOperator(hasOperatorName("|"),
                                  hasLHS(ignoringParenImpCasts(declRefExpr(to(enumConstantDecl())))),
                                  hasRHS(ignoringParenImpCasts(declRefExpr(to(enumConstantDecl()))))
                                 ).bind("bitwiseOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}