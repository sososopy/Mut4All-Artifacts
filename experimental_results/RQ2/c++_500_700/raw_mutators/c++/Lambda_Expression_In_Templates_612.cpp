//header file
#pragma once
#include "Mutator_base.h"

/**
 * lambda_expression_in_templates_612
 */ 
class MutatorFrontendAction_612 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(612)

private:
    class MutatorASTConsumer_612 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_612(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_612.h"

// ========================================================================================================
#define MUT612_OUTPUT 1

void MutatorFrontendAction_612::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
            auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), LE->getSourceRange());
            std::string mutatedLambda = "decltype(" + lambdaSource + ".*(&decltype(" + lambdaSource + ")::operator()))";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), mutatedLambda);
        }
    }
}

void MutatorFrontendAction_612::MutatorASTConsumer_612::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdaMatcher = functionDecl(hasDescendant(lambdaExpr().bind("LambdaExpr"))).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}