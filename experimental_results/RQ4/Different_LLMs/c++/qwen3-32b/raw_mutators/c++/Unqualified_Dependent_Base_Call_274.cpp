//header file
#pragma once
#include "Mutator_base.h"

/**
 * Unqualified_Dependent_Base_Call_274
 */ 
class MutatorFrontendAction_274 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(274)

private:
    class MutatorASTConsumer_274 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_274(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Unqualified_Dependent_Base_Call_274.h"

// ========================================================================================================
#define MUT274_OUTPUT 1

void MutatorFrontendAction_274::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("call")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        auto *ME = dyn_cast<clang::MemberExpr>(CE->getCallee());
        if (!ME || !ME->hasQualifier())
            return;

        clang::NestedNameSpecifier *NNS = ME->getQualifier();
        if (!NNS->isDependent())
            return;

        auto *MD = dyn_cast<clang::CXXMethodDecl>(ME->getDeclContext());
        if (!MD)
            return;

        auto *RD = MD->getParent();
        if (!RD || !RD->isTemplateInstantiation())
            return;

        bool hasDependentBase = false;
        for (const auto *Base : RD->bases()) {
            if (Base->getType()->isTemplateTypeParmType()) {
                hasDependentBase = true;
                break;
            }
        }
        if (!hasDependentBase)
            return;

        SourceRange qualifierRange = ME->getQualifierLoc().getSourceRange();
        Rewrite.RemoveText(qualifierRange);
    }
}

void MutatorFrontendAction_274::MutatorASTConsumer_274::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = callExpr(
        inMethod(),
        callee(memberExpr(hasNestedNameSpecifier(dependentNameType())))
    ).bind("call");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}