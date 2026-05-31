//header file
#pragma once
#include "Mutator_base.h"

/**
 * switch_statement_variable_declaration_issue_675
 */ 
class MutatorFrontendAction_675 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(675)

private:
    class MutatorASTConsumer_675 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_675(Rewriter &R) : TheRewriter(R) {}
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
        std::string switchVarName;
    };
};

//source file
#include "../include/switch_statement_variable_declaration_issue_675.h"

// ========================================================================================================
#define MUT675_OUTPUT 1

void MutatorFrontendAction_675::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SW = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
        if (!SW || !Result.Context->getSourceManager().isWrittenInMainFile(SW->getSwitchLoc()))
            return;

        const Expr *Cond = SW->getCond();
        if (const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(Cond->IgnoreParenCasts())) {
            const VarDecl *VD = dyn_cast<VarDecl>(DRE->getDecl());
            if (VD && !VD->isLocalVarDeclOrParm()) {
                // Attempt to find a suitable local variable
                const FunctionDecl *FD = dyn_cast<FunctionDecl>(SW->getFunctionScopeDecl());
                if (FD) {
                    for (const auto *Param : FD->parameters()) {
                        if (Param->getType() == VD->getType()) {
                            switchVarName = Param->getNameAsString();
                            break;
                        }
                    }
                    if (switchVarName.empty()) {
                        for (const auto *Decl : FD->decls()) {
                            if (const auto *LocalVar = dyn_cast<VarDecl>(Decl)) {
                                if (LocalVar->getType() == VD->getType()) {
                                    switchVarName = LocalVar->getNameAsString();
                                    break;
                                }
                            }
                        }
                    }
                    if (switchVarName.empty()) {
                        // Declare a new variable at the beginning of the function
                        std::string newVarDecl = VD->getType().getAsString() + " " + VD->getNameAsString() + " = 0; /*mut675*/\n";
                        Rewrite.InsertTextBefore(FD->getBody()->getBeginLoc().getLocWithOffset(1), newVarDecl);
                        switchVarName = VD->getNameAsString();
                    }
                    if (!switchVarName.empty()) {
                        Rewrite.ReplaceText(DRE->getSourceRange(), switchVarName);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_675::MutatorASTConsumer_675::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = switchStmt().bind("SwitchStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}