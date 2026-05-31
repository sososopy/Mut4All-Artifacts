//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Pack_Expansion_In_Attribute_383
 */ 
class MutatorFrontendAction_383 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(383)

private:
    class MutatorASTConsumer_383 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_383(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Pack_Expansion_In_Attribute_383.h"

// ========================================================================================================
#define MUT383_OUTPUT 1

void MutatorFrontendAction_383::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                           FD->getLocation()))
            return;

        for (const auto *A : FD->getAttrs()) {
            if (const auto *DiagAttr = dyn_cast<clang::DiagnoseIfAttr>(A)) {
                clang::Expr *Cond = DiagAttr->getCondition();
                if (clang::PackExpansionExpr *PE = clang::dyn_cast<clang::PackExpansionExpr>(Cond)) {
                    SourceLocation ellipsisLoc = PE->getEllipsisLoc();
                    if (ellipsisLoc.isValid()) {
                        Rewrite.RemoveText(ellipsisLoc, 3);
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_383::MutatorASTConsumer_383::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(
        isFunctionTemplateSpecialization(),
        hasAttr(attr::DiagnoseIf)
    ).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}