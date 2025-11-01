//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Instantiation_With_Local_Lambda_433
 */ 
class MutatorFrontendAction_433 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(433)

private:
    class MutatorASTConsumer_433 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_433(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Instantiation_With_Local_Lambda_433.h"

// ========================================================================================================
#define MUT433_OUTPUT 1

void MutatorFrontendAction_433::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        auto lambdaSourceRange = LambdaExpr->getSourceRange();
        auto lambdaSourceText = Lexer::getSourceText(CharSourceRange::getTokenRange(lambdaSourceRange),
                                                     *Result.SourceManager, Result.Context->getLangOpts());

        if (lambdaSourceText.find("[]") != std::string::npos) {
            std::string mutatedLambda = lambdaSourceText;
            size_t capturePos = mutatedLambda.find("[]");
            mutatedLambda.replace(capturePos, 2, "[=]");

            Rewrite.ReplaceText(lambdaSourceRange, mutatedLambda);
        }
    }
}

void MutatorFrontendAction_433::MutatorASTConsumer_433::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdaMatcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}