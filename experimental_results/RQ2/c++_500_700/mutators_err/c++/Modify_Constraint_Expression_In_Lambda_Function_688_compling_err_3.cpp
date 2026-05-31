//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_constraint_expression_in_lambda_function_688
 */ 
class MutatorFrontendAction_688 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(688)

private:
    class MutatorASTConsumer_688 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_688(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_constraint_expression_in_lambda_function_688.h"

// ========================================================================================================
#define MUT688_OUTPUT 1

void MutatorFrontendAction_688::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        //Filter nodes in header files
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        //Get the source code text of target node
        auto requiresClause = LambdaExpr->getTrailingRequiresClause();
        if (!requiresClause)
            return;

        auto sourceRange = requiresClause->getSourceRange();
        auto originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

        //Perform mutation on the source code text by applying string replacement
        std::string modifiedText = "requires(int t) { function1(t); function2(); ++t; }";
        std::string mutatedText = originalText + " /*mut688*/ " + modifiedText;

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}

void MutatorFrontendAction_688::MutatorASTConsumer_688::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasRequiresClause()).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}