//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incorrect_Array_Initialization_Order_299
 */ 
class MutatorFrontendAction_299 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(299)

private:
    class MutatorASTConsumer_299 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_299(Rewriter &R) : TheRewriter(R) {}
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
        const VarDecl *staticVarDecl = nullptr;
    };
};

//source file
#include "../include/Incorrect_Array_Initialization_Order_299.h"

// ========================================================================================================
#define MUT299_OUTPUT 1

void MutatorFrontendAction_299::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
            return;

        if (auto *initExpr = VD->getInit()) {
            if (auto *binOp = dyn_cast<clang::BinaryOperator>(initExpr)) {
                if (binOp->getOpcode() == clang::BO_Assign) {
                    if (auto *lhs = dyn_cast<clang::DeclRefExpr>(binOp->getLHS())) {
                        if (auto *rhs = dyn_cast<clang::DeclRefExpr>(binOp->getRHS())) {
                            if (staticVarDecl) {
                                std::string staticVarName = staticVarDecl->getNameAsString();
                                std::string arrayInit = "static " + staticVarName + " = " + lhs->getDecl()->getNameAsString() + ";";
                                Rewrite.InsertTextBefore(VD->getBeginLoc(), arrayInit + "\n");
                            }
                        }
                    }
                }
            }
        }
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("staticVarDecl")) {
        staticVarDecl = VD;
    }
}

void MutatorFrontendAction_299::MutatorASTConsumer_299::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher arrayMatcher = varDecl(hasInitializer(binaryOperator(hasOperatorName("=")))).bind("arrayDecl");
    DeclarationMatcher staticVarMatcher = varDecl(hasStaticStorageDuration()).bind("staticVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(arrayMatcher, &callback);
    matchFinder.addMatcher(staticVarMatcher, &callback);
    matchFinder.matchAST(Context);
}