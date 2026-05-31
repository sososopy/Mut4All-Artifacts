//header file
#pragma once
#include "Mutator_base.h"

/**
 * Unexpanded_Pack_In_Concept_Constraint_135
 */ 
class MutatorFrontendAction_UnexpandedPackInConceptConstraint_135 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(UnexpandedPackInConceptConstraint_135)

private:
    class MutatorASTConsumer_UnexpandedPackInConceptConstraint_135 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_UnexpandedPackInConceptConstraint_135(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Unexpanded_Pack_In_Concept_Constraint_135.h"

// ========================================================================================================
#define MUTUnexpandedPackInConceptConstraint_135_OUTPUT 1

void MutatorFrontendAction_UnexpandedPackInConceptConstraint_135::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *PE = Result.Nodes.getNodeAs<clang::PackExpansionExpr>("packExpansion")) {
        if (!PE || !Result.Context->getSourceManager().isWrittenInMainFile(PE->getLocation()))
            return;
        SourceLocation ellipsisLoc = PE->getEllipsisLoc();
        if (ellipsisLoc.isValid()) {
            Rewrite.RemoveText(ellipsisLoc);
        }
    }
}

void MutatorFrontendAction_UnexpandedPackInConceptConstraint_135::MutatorASTConsumer_UnexpandedPackInConceptConstraint_135::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = packExpansionExpr().bind("packExpansion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}