//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_return_types_4
 */ 
class MutatorFrontendAction_4 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(4)

private:
    class MutatorASTConsumer_4 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_4(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/modify_lambda_return_types_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        const auto *LambdaType = LambdaExpr->getCallOperator()->getReturnType().getTypePtrOrNull();
        if (LambdaType && LambdaType->isFunctionType()) {
            return; // Skip if already a function type
        }

        std::string originalReturnType = LambdaType->getAsString();
        std::string mutatedReturnType = originalReturnType + "(" + originalReturnType + ")";

        SourceLocation arrowLoc = LambdaExpr->getIntroducerRange().getEnd();
        SourceLocation returnTypeLoc = Lexer::findLocationAfterToken(
            arrowLoc, tok::arrow, *Result.SourceManager, Result.Context->getLangOpts(), false);

        if (returnTypeLoc.isValid()) {
            Rewrite.ReplaceText(returnTypeLoc, originalReturnType.length(), mutatedReturnType);
        }
    }
}
  
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}