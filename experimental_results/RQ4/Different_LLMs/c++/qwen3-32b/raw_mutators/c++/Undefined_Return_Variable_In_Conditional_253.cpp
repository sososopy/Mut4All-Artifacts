//header file
#pragma once
#include "Mutator_base.h"

/**
 * Undefined_Return_Variable_In_Conditional_253
 */ 
class MutatorFrontendAction_253 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(253)

private:
    class MutatorASTConsumer_253 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_253(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/undefined_return_variable_in_conditional_253.h"

// ========================================================================================================
#define MUT253_OUTPUT 1

void MutatorFrontendAction_253::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>("returnStmt")) {
        if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(RS->getBeginLoc()))
            return;
        
        auto *RetVal = RS->getReturnValue();
        if (!RetVal)
            return;
        
        if (auto *DRE = dyn_cast<clang::DeclRefExpr>(RetVal)) {
            auto *VarDecl = dyn_cast<clang::VarDecl>(DRE->getDecl());
            if (!VarDecl)
                return;
            
            auto *RSFunc = RS->getDeclContext()->getEnclosingFunction();
            auto *VarFunc = VarDecl->getDeclContext()->getEnclosingFunction();
            if (!RSFunc || !VarFunc || RSFunc != VarFunc)
                return;
            
            if (!Result.Context->getSourceManager().isWrittenInMainFile(VarDecl->getLocation()))
                return;
            
            std::string declText = stringutils::rangetoStr(*Result.SourceManager, VarDecl->getSourceRange());
            std::string commentText = "// " + declText + " // Declaration removed to leave variable undefined.\n";
            Rewrite.ReplaceText(VarDecl->getSourceRange(), commentText);
        }
    }
}

void MutatorFrontendAction_253::MutatorASTConsumer_253::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = returnStmt(
        hasAncestor(ifStmt().hasElse(stmt())),
        hasReturnValue(declRefExpr())
    ).bind("returnStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}