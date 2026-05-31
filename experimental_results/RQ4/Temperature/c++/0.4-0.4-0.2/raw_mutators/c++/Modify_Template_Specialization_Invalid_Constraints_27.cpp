//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_specialization_invalid_constraints_27
 */ 
class MutatorFrontendAction_27 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(27)

private:
    class MutatorASTConsumer_27 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_27(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_specialization_invalid_constraints_27.h"

// ========================================================================================================
#define MUT27_OUTPUT 1

void MutatorFrontendAction_27::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ConceptDecl = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
        if (!ConceptDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                ConceptDecl->getLocation()))
            return;

        auto constraintExpr = ConceptDecl->getConstraintExpr();
        if (!constraintExpr)
            return;

        auto originalConstraint = stringutils::rangetoStr(*(Result.SourceManager),
                                                          constraintExpr->getSourceRange());
        std::string mutatedConstraint = "0"; // Invalid constraint

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(constraintExpr->getSourceRange()), mutatedConstraint);
    }
}

void MutatorFrontendAction_27::MutatorASTConsumer_27::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}