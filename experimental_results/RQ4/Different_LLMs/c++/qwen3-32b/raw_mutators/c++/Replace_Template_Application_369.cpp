//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Application_369
 */ 
class MutatorFrontendAction_Replace_Template_Application_369 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Replace_Template_Application_369)

private:
    class MutatorASTConsumer_Replace_Template_Application_369 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Replace_Template_Application_369(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Application_369.h"

// ========================================================================================================
#define MUTReplace_Template_Application_369_OUTPUT 1

void MutatorFrontendAction_Replace_Template_Application_369::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ES = Result.Nodes.getNodeAs<clang::ExprStmt>("exprStmt")) {
        if (!ES || !Result.Context->getSourceManager().isWrittenInMainFile(ES->getBeginLoc()))
            return;
        Rewrite.ReplaceText(ES->getSourceRange(), "Alias(42);");
    }
}
  
void MutatorFrontendAction_Replace_Template_Application_369::MutatorASTConsumer_Replace_Template_Application_369::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = exprStmt(hasDescendant(callExpr())).bind("exprStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}