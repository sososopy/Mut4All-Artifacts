//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_Capture_In_Requires_Clause_140
 */ 
class MutatorFrontendAction_140 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(140)

private:
    class MutatorASTConsumer_140 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_140(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Lambda_Capture_In_Requires_Clause_140.h"

// ========================================================================================================
#define MUT140_OUTPUT 1

void MutatorFrontendAction_140::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->hasRequiresClause())
            return;
        clang::RequiresExpr *RE = FD->getRequiresClause();
        if (!RE)
            return;
        SourceLocation start = RE->getRequiresLoc();
        SourceLocation end = RE->getRParenLoc();
        if (start.isInvalid() || end.isInvalid())
            return;
        SourceManager &SM = Rewrite.getSourceMgr();
        const LangOptions &LO = SM.getLangOpts();
        std::string requiresText = Lexer::getSourceText(CharSourceRange::getCharRange(start, end), SM, LO);
        if (requiresText.size() < 9)
            return;
        std::string condition = requiresText.substr(9, requiresText.size() - 10);
        if (condition.empty())
            return;
        std::string newRequiresClause = "requires [x=42] { return " + condition + " && x > 42; }()";
        Rewrite.ReplaceText(start, requiresText.size(), newRequiresClause);
    }
}

void MutatorFrontendAction_140::MutatorASTConsumer_140::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasRequiresClause()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}