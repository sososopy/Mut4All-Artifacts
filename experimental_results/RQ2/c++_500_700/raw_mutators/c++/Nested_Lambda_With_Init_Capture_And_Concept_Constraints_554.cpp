//header file
#pragma once
#include "Mutator_base.h"

/**
 * nested_lambda_with_init_capture_and_concept_constraints_554
 */ 
class MutatorFrontendAction_554 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(554)

private:
    class MutatorASTConsumer_554 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_554(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/nested_lambda_with_init_capture_and_concept_constraints_554.h"

// ========================================================================================================
#define MUT554_OUTPUT 1

void MutatorFrontendAction_554::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), LambdaExpr->getSourceRange());

        std::string nestedLambda = "[captured{input}]() requires true { return captured + value; };";
        size_t pos = lambdaSource.find("{");
        if (pos != std::string::npos) {
            lambdaSource.insert(pos + 1, nestedLambda);
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), lambdaSource);
    }
}

void MutatorFrontendAction_554::MutatorASTConsumer_554::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasAncestor(functionTemplateDecl())).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}