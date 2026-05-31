//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Var_With_Undeclared_In_Constexpr_Switch_102
 */ 
class MutatorFrontendAction_102 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(102)

private:
    class MutatorASTConsumer_102 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_102(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Var_With_Undeclared_In_Constexpr_Switch_102.h"

// ========================================================================================================
#define MUT102_OUTPUT 1

void MutatorFrontendAction_102::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::SwitchStmt>("switchStmt")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        auto init = MT->getInit();
        if (!init)
            return;
        if (auto *declRef = dyn_cast<clang::DeclRefExpr>(init)) {
            Rewrite.ReplaceText(declRef->getSourceRange(), "undeclared_var");
        }
    }
}
  
void MutatorFrontendAction_102::MutatorASTConsumer_102::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = switchStmt(hasAncestor(functionDecl(isConstexpr())), hasInit(declRefExpr())).bind("switchStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}