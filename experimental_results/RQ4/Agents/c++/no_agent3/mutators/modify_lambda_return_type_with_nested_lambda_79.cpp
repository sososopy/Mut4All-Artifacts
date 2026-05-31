//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Return_Type_With_Nested_Lambda_79
 */ 
class MutatorFrontendAction_79 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(79)

private:
    class MutatorASTConsumer_79 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_79(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_lambda_return_type_with_nested_lambda_79.h"

// ========================================================================================================
#define MUT79_OUTPUT 1

void MutatorFrontendAction_79::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        auto *InnerLambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("InnerLambdaExpr");
        if (!InnerLambda)
            return;

        // Get the source code text of target node
        auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), LambdaExpr->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string modifiedLambdaSource = lambdaSource;
        size_t pos = modifiedLambdaSource.find("return 0;");
        if (pos != std::string::npos) {
            modifiedLambdaSource.replace(pos, 9, "return Example<int>::value;");
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), modifiedLambdaSource);
    }
}

void MutatorFrontendAction_79::MutatorASTConsumer_79::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;

    // Define ASTMatchers to identify the target AST node for mutation
    auto lambdaMatcher = lambdaExpr(hasDescendant(lambdaExpr().bind("InnerLambdaExpr"))).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}