//header file
#pragma once
#include "Mutator_base.h"
#include <regex>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

/**
 * incorrect_lambda_return_type_4
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
#include "../include/incorrect_lambda_return_type_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
        //Filter nodes in header files
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        if (LambdaExpr->hasExplicitResultType()) {
            //Get the source code text of target node
            auto BeginLoc = LambdaExpr->getBeginLoc();
            auto EndLoc = LambdaExpr->getBody()->getBeginLoc().getLocWithOffset(-1);
            auto SourceRange = clang::SourceRange(BeginLoc, EndLoc);
            auto OriginalText = clang::Lexer::getSourceText(
                clang::CharSourceRange::getTokenRange(SourceRange),
                *Result.SourceManager, clang::LangOptions());

            //Perform mutation on the source code text by applying string replacement
            std::string MutatedText = std::regex_replace(
                OriginalText.str(), std::regex(R"(->\s*[\w:]+)"),
                "-> int(int)");

            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(SourceRange, MutatedText);
        }
    }
}

void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr(hasExplicitReturnType()).bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}