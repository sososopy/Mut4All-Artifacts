//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/ExprCXX.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"

/**
 * Modify_Requires_Expression_With_Concepts_And_Inline_Lambda_105
 */ 
class MutatorFrontendAction_105 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(105)

private:
    class MutatorASTConsumer_105 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_105(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_requires_expression_with_concepts_and_inline_lambda_105.h"

// ========================================================================================================
#define MUT105_OUTPUT 1

void MutatorFrontendAction_105::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        auto *RequiresExpr = Result.Nodes.getNodeAs<clang::RequiresExpr>("Requires");
        if (!RequiresExpr)
            return;

        auto sourceManager = Result.SourceManager;
        auto requiresRange = RequiresExpr->getSourceRange();
        auto requiresText = clang::tooling::getText(*sourceManager, requiresRange);

        // Perform mutation: Swap the order of begin and end in the requires expression
        if (requiresText.find("begin") != std::string::npos && requiresText.find("end") != std::string::npos) {
            std::swap(requiresText[requiresText.find("begin")], requiresText[requiresText.find("end")]);
        }

        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(requiresRange), requiresText);
    }
}

void MutatorFrontendAction_105::MutatorASTConsumer_105::HandleTranslationUnit(ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    auto lambdaMatcher = clang::ast_matchers::lambdaExpr(
        clang::ast_matchers::hasDescendant(clang::ast_matchers::requiresExpr().bind("Requires"))
    ).bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}