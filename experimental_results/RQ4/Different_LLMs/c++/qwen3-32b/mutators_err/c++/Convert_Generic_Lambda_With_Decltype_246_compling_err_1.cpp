//header file
#pragma once
#include "Mutator_base.h"

/**
 * Convert_Generic_Lambda_With_Decltype_246
 */ 
class MutatorFrontendAction_246 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(246)

private:
    class MutatorASTConsumer_246 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_246(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite), hasProcessedLambda(false) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        bool hasProcessedLambda;
    };
};

//source file
#include "../include/Convert_Generic_Lambda_With_Decltype_246.h"

// ========================================================================================================
#define MUT246_OUTPUT 1

void MutatorFrontendAction_246::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!lambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(lambdaExpr->getBeginLoc()))
            return;

        CXXMethodDecl *callOp = lambdaExpr->getCallOperator();
        if (!callOp)
            return;

        const auto &params = callOp->parameters();
        if (params.empty()) {
            SourceRange lambdaRange = lambdaExpr->getSourceRange();
            std::string originalLambdaText = Lexer::getSourceText(CharSourceRange::getTokenRange(lambdaRange), 
                                                                  *Result.SourceManager, 
                                                                  Result.Context->getLangOpts()).str();
            std::string newLambdaText = "decltype(" + originalLambdaText + ")";
            Rewrite.ReplaceText(lambdaRange, newLambdaText);
            hasProcessedLambda = true;
            return;
        }

        std::string newParams;
        for (size_t i = 0; i < params.size(); ++i) {
            ParmVarDecl *param = params[i];
            newParams += "auto " + param->getNameAsString();
            if (i != params.size() - 1) {
                newParams += ", ";
            }
        }
        std::string newParamList = "(" + newParams + ")";

        SourceRange lambdaRange = lambdaExpr->getSourceRange();
        std::string originalLambdaText = Lexer::getSourceText(CharSourceRange::getTokenRange(lambdaRange), 
                                                              *Result.SourceManager, 
                                                              Result.Context->getLangOpts()).str();

        size_t openParenPos = originalLambdaText.find('(');
        size_t closeParenPos = originalLambdaText.find(')');
        if (openParenPos == std::string::npos || closeParenPos == std::string::npos) {
            return;
        }

        std::string modifiedLambdaText = originalLambdaText.substr(0, openParenPos + 1) + 
                                         newParamList.substr(1, newParamList.size() - 2) + 
                                         originalLambdaText.substr(closeParenPos);

        std::string newLambdaText = "decltype(" + modifiedLambdaText + ")";
        Rewrite.ReplaceText(lambdaRange, newLambdaText);
        hasProcessedLambda = true;
    } else if (auto *functionDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("function")) {
        if (!hasProcessedLambda) {
            if (functionDecl->hasBody()) {
                Stmt *body = functionDecl->getBody();
                if (body) {
                    SourceLocation endLoc = body->getEndLoc();
                    std::string newLambda = "auto lambda = [ ](auto x) { return x; };";
                    Rewrite.InsertText(endLoc, "\n" + newLambda + " /*mut246*/\n");
                }
            }
        }
    }
}
  
void MutatorFrontendAction_246::MutatorASTConsumer_246::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher lambdaMatcher = lambdaExpr().bind("lambda");
    DeclarationMatcher functionMatcher = functionDecl(hasBody(compoundStmt())).bind("function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}