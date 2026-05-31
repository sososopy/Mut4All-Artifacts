//source file
#include "../include/Transform_If_Else_To_Conditional_416.h"

// ========================================================================================================
#define MUT416_OUTPUT 1

void MutatorFrontendAction_416::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *IfStmt = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt")) {
        //Filter nodes in header files
        if (!IfStmt || !Result.Context->getSourceManager().isWrittenInMainFile(
                IfStmt->getBeginLoc()))
            return;

        if (const auto *ThenStmt = dyn_cast<clang::ReturnStmt>(IfStmt->getThen())) {
            if (const auto *ElseStmt = dyn_cast<clang::ReturnStmt>(IfStmt->getElse())) {
                //Get the source code text of target node
                auto ThenExpr = ThenStmt->getRetValue();
                auto ElseExpr = ElseStmt->getRetValue();
                if (!ThenExpr || !ElseExpr)
                    return;

                std::string CondExpr = Lexer::getSourceText(CharSourceRange::getTokenRange(IfStmt->getCond()->getSourceRange()), 
                                                            *Result.SourceManager, Result.Context->getLangOpts()).str();
                std::string ThenExprStr = Lexer::getSourceText(CharSourceRange::getTokenRange(ThenExpr->getSourceRange()), 
                                                               *Result.SourceManager, Result.Context->getLangOpts()).str();
                std::string ElseExprStr = Lexer::getSourceText(CharSourceRange::getTokenRange(ElseExpr->getSourceRange()), 
                                                               *Result.SourceManager, Result.Context->getLangOpts()).str();

                //Perform mutation on the source code text by applying string replacement
                std::string TernaryExpr = "/*mut416*/return " + CondExpr + " ? " + ThenExprStr + " : " + ElseExprStr + ";";
                
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(IfStmt->getSourceRange()), TernaryExpr);
            }
        }
    }
}

void MutatorFrontendAction_416::MutatorASTConsumer_416::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = ifStmt(hasThen(returnStmt()), hasElse(returnStmt())).bind("IfStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}