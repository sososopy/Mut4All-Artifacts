//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Conditional_Operator_With_If_Else_268
 */ 
class MutatorFrontendAction_268 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(268)

private:
    class MutatorASTConsumer_268 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_268(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const ConditionalOperator*, std::string> originalCodeMap;
    };
};

//source file
#include "../include/replace_conditional_operator_with_if_else_268.h"

// ========================================================================================================
#define MUT268_OUTPUT 1

void MutatorFrontendAction_268::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CO = Result.Nodes.getNodeAs<clang::ConditionalOperator>("CondOp")) {
        if (!CO || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CO->getLocation()))
            return;

        SourceManager &SM = Rewrite.getSourceMgr();
        LangOptions LangOpts;
        LangOpts.CPlusPlus = true;
        std::string condition = Lexer::getSourceText(CharSourceRange::getTokenRange(CO->getCond()->getSourceRange()), SM, LangOpts).str();
        std::string trueExpr = Lexer::getSourceText(CharSourceRange::getTokenRange(CO->getTrueExpr()->getSourceRange()), SM, LangOpts).str();
        std::string falseExpr = Lexer::getSourceText(CharSourceRange::getTokenRange(CO->getFalseExpr()->getSourceRange()), SM, LangOpts).str();

        std::string ifElseCode = "/*mut268*/if (" + condition + ") { " + trueExpr + "; } else { " + falseExpr + "; }";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CO->getSourceRange()), ifElseCode);
    }
}

void MutatorFrontendAction_268::MutatorASTConsumer_268::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = conditionalOperator().bind("CondOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}