//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Requires_Clause_355
 */ 
class MutatorFrontendAction_355 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(355)

private:
    class MutatorASTConsumer_355 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_355(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalid_Requires_Clause_355.h"

// ========================================================================================================
#define MUT355_OUTPUT 1

void MutatorFrontendAction_355::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::Stmt>("stmt")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getBeginLoc()))
            return;

        // Skip branching statements
        if (isa<clang::IfStmt>(MT) || isa<clang::ForStmt>(MT) || 
            isa<clang::WhileStmt>(MT) || isa<clang::SwitchStmt>(MT) || 
            isa<clang::DoStmt>(MT)) {
            return;
        }

        // Generate malformed requires clause
        std::string invalidRequires = "requires { requires (T; }";
        Rewrite.ReplaceText(MT->getSourceRange(), invalidRequires);
    }
}

void MutatorFrontendAction_355::MutatorASTConsumer_355::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = stmt(hasAncestor(functionDecl())).bind("stmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}