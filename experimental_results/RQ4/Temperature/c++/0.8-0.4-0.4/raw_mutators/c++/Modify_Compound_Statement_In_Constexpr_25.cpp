//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_compound_statement_in_constexpr_25
 */ 
class MutatorFrontendAction_25 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(25)

private:
    class MutatorASTConsumer_25 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_25(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_compound_statement_in_constexpr_25.h"

// ========================================================================================================
#define MUT25_OUTPUT 1

void MutatorFrontendAction_25::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CS = Result.Nodes.getNodeAs<clang::CompoundStmt>("compoundStmt")) {
        if (!CS || !Result.Context->getSourceManager().isWrittenInMainFile(CS->getLBracLoc()))
            return;

        auto &SM = *Result.SourceManager;
        auto &LangOpts = Result.Context->getLangOpts();
        auto StartLoc = CS->getLBracLoc().getLocWithOffset(1);
        auto EndLoc = CS->getRBracLoc().getLocWithOffset(-1);

        std::string OriginalText = Lexer::getSourceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), SM, LangOpts).str();
        std::string MutatedText = OriginalText + " float *ptr = nullptr; x += reinterpret_cast<int>(ptr);";

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), MutatedText);
    }
}

void MutatorFrontendAction_25::MutatorASTConsumer_25::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = compoundStmt(hasParent(ifStmt(hasAncestor(functionDecl(isConstexpr()))))).bind("compoundStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}