//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_return_type_61
 */ 
class MutatorFrontendAction_61 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(61)

private:
    class MutatorASTConsumer_61 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_61(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_lambda_return_type_61.h"

// ========================================================================================================
#define MUT61_OUTPUT 1

void MutatorFrontendAction_61::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        const auto *TypeInfo = LambdaExpr->getLambdaClass()->getLambdaCallOperator()->getReturnTypeSourceRange();
        if (!TypeInfo)
            return;

        auto OriginalSource = stringutils::rangetoStr(*(Result.SourceManager), *TypeInfo);
        std::string MutatedSource = "decltype(auto)";

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(*TypeInfo), MutatedSource);
    }
}
  
void MutatorFrontendAction_61::MutatorASTConsumer_61::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasType(autoType())).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}