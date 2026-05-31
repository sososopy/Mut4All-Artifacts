//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Var_With_Function_Ptr_277
 */ 
class MutatorFrontendAction_277 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(277)

private:
    class MutatorASTConsumer_277 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_277(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::VarDecl*> candidateVars;
        bool dummyFuncAdded = false;
    };
};

//source file
#include "../include/Replace_Var_With_Function_Ptr_277.h"

// ========================================================================================================
#define MUT277_OUTPUT 1

void MutatorFrontendAction_277::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *expr = Result.Nodes.getNodeAs<clang::Expr>("expr")) {
        if (!Result.Context->getSourceManager().isWrittenInMainFile(expr->getBeginLoc()))
            return;

        clang::VarDecl *varDecl = nullptr;

        if (auto *unaryOp = dyn_cast<clang::UnaryOperator>(expr)) {
            auto *declRef = dyn_cast<clang::DeclRefExpr>(unaryOp->getSubExpr());
            if (declRef) {
                varDecl = dyn_cast<clang::VarDecl>(declRef->getDecl());
            }
        } else if (auto *arraySub = dyn_cast<clang::ArraySubscriptExpr>(expr)) {
            auto *baseExpr = arraySub->getBase();
            auto *declRef = dyn_cast<clang::DeclRefExpr>(baseExpr);
            if (declRef) {
                varDecl = dyn_cast<clang::VarDecl>(declRef->getDecl());
            }
        }

        if (varDecl && !varDecl->getType()->isPointerType() && !varDecl->getType()->isArrayType()) {
            candidateVars.insert(varDecl);
        }
    }

    if (candidateVars.empty())
        return;

    for (const auto *varDecl : candidateVars) {
        clang::SourceManager &SM = Rewrite.getSourceMgr();
        clang::SourceLocation typeLoc = varDecl->getTypeSourceInfo()->getTypeLoc().getBeginLoc();
        clang::SourceRange typeRange = varDecl->getTypeSourceInfo()->getTypeLoc().getSourceRange();

        if (SM.isInMainFile(typeLoc)) {
            std::string newType = "void (*)()";
            Rewrite.ReplaceText(typeRange, newType);
        }

        if (!dummyFuncAdded) {
            clang::SourceLocation insertLoc = SM.getLocWithOffset(SM.getMainFileID(), 0);
            Rewrite.InsertText(insertLoc, "void dummy_func();\n", true, true);
            dummyFuncAdded = true;
        }
    }
}
  
void MutatorFrontendAction_277::MutatorASTConsumer_277::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = anyOf(
        unaryOperator(hasOperatorName("*"), hasUnaryOperand(declRefExpr(to(varDecl())))),
        arraySubscriptExpr(hasBase(declRefExpr(to(varDecl()))))
    ).bind("expr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}