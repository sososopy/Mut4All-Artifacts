//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/Expr.h"

/**
 * Introduce_Requires_Clause_In_Concept_108
 */ 
class MutatorFrontendAction_108 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(108)

private:
    class MutatorASTConsumer_108 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_108(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT108_OUTPUT 1

void MutatorFrontendAction_108::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;

        auto *TPL = CD->getTemplateParameters();
        if (!TPL || TPL->size() == 0)
            return;

        auto *TParam = TPL->getParam(0);
        auto *TDecl = dyn_cast<clang::TemplateTypeParmDecl>(TParam);
        if (!TDecl)
            return;

        std::string TName = TDecl->getNameAsString();
        if (TName.empty())
            return;

        auto *CE = CD->getConstraintExpr();
        if (!CE)
            return;

        SourceRange constraintRange = CE->getSourceRange();
        if (!constraintRange.isValid())
            return;

        std::string newConstraint = R"(
[]{  
    return requires(" + TName + " t) {  
        { t } -> satisfies_something;  
    };  
}())";

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(constraintRange), newConstraint);
    }
}

void MutatorFrontendAction_108::MutatorASTConsumer_108::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl(hasTemplateParameters(anything())).bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}