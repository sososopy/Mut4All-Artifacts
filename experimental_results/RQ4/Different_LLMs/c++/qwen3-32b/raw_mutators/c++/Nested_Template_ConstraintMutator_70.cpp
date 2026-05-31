//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Template_ConstraintMutator_70
 */ 
class MutatorFrontendAction_70 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(70)

private:
    class MutatorASTConsumer_70 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_70(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Nested_Template_ConstraintMutator_70.h"

// ========================================================================================================
#define MUT70_OUTPUT 1

void MutatorFrontendAction_70::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("concept")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;

        auto *RE = CD->getRequiresClause();
        if (!RE)
            return;

        auto *Body = RE->getBody();
        if (!Body)
            return;

        if (auto *TTE = dyn_cast<clang::TypeTraitExpr>(Body)) {
            if (TTE->getTypeTrait() != clang::TypeTrait::TT_void_t)
                return;

            clang::TypeSourceInfo *TSI = TTE->getArg(0);
            if (!TSI)
                return;

            SourceRange ArgRange = TSI->getTypeLoc().getSourceRange();
            std::string newText = "T::template deeply_nested<>";
            Rewrite.ReplaceText(ArgRange, newText);
        }
    }
}
  
void MutatorFrontendAction_70::MutatorASTConsumer_70::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl(
        hasRequiresClause(
            requiresExpr(
                hasBody(
                    typeTraitExpr(
                        hasTypeTrait(clang::TypeTrait::TT_void_t),
                        hasArgument(0, dependentNameType())
                    )
                )
            )
        )
    ).bind("concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}