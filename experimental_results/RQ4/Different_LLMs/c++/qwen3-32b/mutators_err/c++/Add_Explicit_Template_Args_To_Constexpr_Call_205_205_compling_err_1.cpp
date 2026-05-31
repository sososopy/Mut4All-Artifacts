//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Explicit_Template_Args_To_Constexpr_Call_205
 */ 
class MutatorFrontendAction_205 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(205)

private:
    class MutatorASTConsumer_205 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_205(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Explicit_Template_Args_To_Constexpr_Call_205.h"

// ========================================================================================================
#define MUT205_OUTPUT 1

void MutatorFrontendAction_205::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("call")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        const FunctionDecl *FD = CE->getDirectCallee();
        if (!FD || FD->getTemplateSpecializationKind() != clang::TSK_Undeclared || CE->getNumTemplateArgs() != 0)
            return;

        const Expr *Callee = CE->getCallee();
        SourceLocation insertLoc = Callee->getEndLoc();
        if (insertLoc.isInvalid())
            return;

        Rewrite.InsertText(insertLoc, "<1>", true, true);
    }
}

void MutatorFrontendAction_205::MutatorASTConsumer_205::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = callExpr(
        callee(functionDecl(isConstexpr()))
    ).bind("call");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}