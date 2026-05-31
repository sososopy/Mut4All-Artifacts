//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Scope_Resolution_In_C_356
 */ 
class MutatorFrontendAction_356 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(356)

private:
    class MutatorASTConsumer_356 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_356(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Scope_Resolution_In_C_356.h"

// ========================================================================================================
#define MUT356_OUTPUT 1

void MutatorFrontendAction_356::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *S = Result.Nodes.getNodeAs<clang::Stmt>("stmt")) {
        // Check if in main file and is a C file
        if (!Result.Context->getSourceManager().isWrittenInMainFile(S->getBeginLoc())) {
            return;
        }
        auto SM = &Result.Context->getSourceManager();
        auto loc = S->getBeginLoc();
        if (SM->isInMainFile(loc)) {
            auto file = SM->getFilename(loc);
            if (!file.endswith(".c")) {
                return;
            }
        } else {
            return;
        }
        // Replace the statement with ::;
        auto range = S->getSourceRange();
        Rewrite.ReplaceText(range, "::;");
    }
}

void MutatorFrontendAction_356::MutatorASTConsumer_356::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = stmt().bind("stmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}