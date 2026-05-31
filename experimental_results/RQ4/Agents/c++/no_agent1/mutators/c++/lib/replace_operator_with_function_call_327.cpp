//source file
#include "../include/replace_operator_with_function_call_327.h"

// ========================================================================================================
#define MUT327_OUTPUT 1

void MutatorFrontendAction_327::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                       BO->getExprLoc()))
            return;

        auto lhsText = Lexer::getSourceText(CharSourceRange::getTokenRange(BO->getLHS()->getSourceRange()), 
                                            *Result.SourceManager, 
                                            Result.Context->getLangOpts());
        auto rhsText = Lexer::getSourceText(CharSourceRange::getTokenRange(BO->getRHS()->getSourceRange()), 
                                            *Result.SourceManager, 
                                            Result.Context->getLangOpts());

        std::string opText = BO->getOpcodeStr().str();
        std::string replacement = "/*mut327*/performOperation(" + lhsText.str() + ", \"" + opText + "\", " + rhsText.str() + ")";
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), replacement);
    }
}

void MutatorFrontendAction_327::MutatorASTConsumer_327::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = binaryOperator(isExpansionInMainFile()).bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}