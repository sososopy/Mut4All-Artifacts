//header file
#pragma once
#include "Mutator_base.h"

/**
 * Swap_Iterators_In_Lambda_138
 */ 
class MutatorFrontendAction_138 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(138)

private:
    class MutatorASTConsumer_138 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_138(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Swap_Iterators_In_Lambda_138.h"

// ========================================================================================================
#define MUT138_OUTPUT 1

void MutatorFrontendAction_138::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        auto &SM = Rewrite.getSourceMgr();
        auto &LangOpts = Rewrite.getLangOpts();
        auto LambdaRange = CharSourceRange::getTokenRange(LambdaExpr->getSourceRange());
        std::string LambdaCode = Lexer::getSourceText(LambdaRange, SM, LangOpts).str();

        size_t pos = LambdaCode.find("{");
        if (pos != std::string::npos) {
            std::string mutation = "if (begin > end) { std::swap(begin, end); }";
            LambdaCode.insert(pos + 1, mutation);
            Rewrite.ReplaceText(LambdaRange, LambdaCode);
        }
    }
}

void MutatorFrontendAction_138::MutatorASTConsumer_138::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasParameter(0, hasType(isIteratorType())), 
                              hasParameter(1, hasType(isIteratorType()))).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}