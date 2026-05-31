//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Pack_Expansion_With_Reference_278
 */ 
class MutatorFrontendAction_278 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(278)

private:
    class MutatorASTConsumer_278 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_278(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Pack_Expansion_With_Reference_278.h"

// ========================================================================================================
#define MUT278_OUTPUT 1

void MutatorFrontendAction_278::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *PE = Result.Nodes.getNodeAs<clang::PackExpansionExpr>("packExpansion")) {
        if (!PE || !Result.Context->getSourceManager().isWrittenInMainFile(PE->getLocation()))
            return;
        
        auto *Parent = PE->getParent();
        while (Parent) {
            if (isa<clang::UnaryExprOrTypeTraitExpr>(Parent) || 
                isa<clang::NoexceptExpr>(Parent) || 
                isa<clang::DecltypeType>(Parent)) {
                break;
            }
            Parent = Parent->getParent();
        }
        
        if (!Parent) return;
        
        auto ellipsisLoc = PE->getEllipsisLoc();
        if (ellipsisLoc.isInvalid()) return;
        
        Rewrite.ReplaceText(ellipsisLoc, 3, "");
    }
}
  
void MutatorFrontendAction_278::MutatorASTConsumer_278::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = packExpansionExpr().bind("packExpansion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}