//header file
#pragma once
#include "Mutator_base.h"

/**
 * Increase_Template_Recursion_Depth_156
 */ 
class MutatorFrontendAction_156 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(156)
private:
    class MutatorASTConsumer_156 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_156(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Increase_Template_Recursion_Depth_156.h"

// ========================================================================================================
#define MUT156_OUTPUT 1

void MutatorFrontendAction_156::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *IfStmt = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt")) {
        if (!IfStmt || !Result.Context->getSourceManager().isWrittenInMainFile(IfStmt->getBeginLoc()))
            return;

        const clang::Expr *Cond = IfStmt->getCond()->IgnoreImplicit();
        if (auto *BO = dyn_cast<clang::BinaryOperator>(Cond)) {
            if (BO->getOpcode() != clang::BO_EQ)
                return;

            clang::Expr *LHS = BO->getLHS()->IgnoreImplicit();
            clang::Expr *RHS = BO->getRHS()->IgnoreImplicit();

            if (auto *DRE = dyn_cast<clang::DeclRefExpr>(LHS)) {
                if (auto *NTTP = dyn_cast<clang::NonTypeTemplateParmDecl>(DRE->getDecl())) {
                    if (auto *IL = dyn_cast<clang::IntegerLiteral>(RHS)) {
                        if (IL->getValue().getSExtValue() == 0) {
                            SourceLocation loc = IL->getLocation();
                            Rewrite.ReplaceText(loc, "1");
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_156::MutatorASTConsumer_156::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = ifStmt(
        hasCondition(binaryOperator(
            hasOperatorName("=="),
            hasLHS(declRefExpr(to(nonTypeTemplateParmDecl()))),
            hasRHS(integerLiteral(equals(0)))
        ))
    ).bind("ifStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}