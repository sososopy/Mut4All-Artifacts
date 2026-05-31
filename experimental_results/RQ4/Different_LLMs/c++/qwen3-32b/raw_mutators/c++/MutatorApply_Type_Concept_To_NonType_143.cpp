//header file
#pragma once
#include "Mutator_base.h"

/**
 * MutatorApply_Type_Concept_To_NonType_143
 */ 
class MutatorFrontendAction_143 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(143)

private:
    class MutatorASTConsumer_143 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_143(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/MutatorApply_Type_Concept_To_NonType_143.h"

// ========================================================================================================
#define MUT143_OUTPUT 1

void MutatorFrontendAction_143::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *funcTemplate = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("funcTemplate")) {
        if (!Result.Context->getSourceManager().isWrittenInMainFile(funcTemplate->getLocation()))
            return;

        auto *ntParam = Result.Nodes.getNodeAs<clang::NonTypeTemplateParmDecl>("conceptRef");
        auto *typeParam = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("typeParam");

        if (!ntParam || !typeParam)
            return;

        auto *conceptRef = ntParam->getConceptReference();
        if (!conceptRef)
            return;

        SourceRange conceptRange = conceptRef->getSourceRange();
        if (conceptRange.isInvalid())
            return;

        std::string replacement = typeParam->getNameAsString();
        Rewrite.ReplaceText(conceptRange, replacement);
    }
}

void MutatorFrontendAction_143::MutatorASTConsumer_143::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        hasTemplateParameterList(
            hasAnyTemplateParameter(
                templateTypeParmDecl().bind("typeParam")
            )
        ),
        hasAnyParameter(
            nonTypeTemplateParmDecl(
                hasConceptReference().bind("conceptRef")
            )
        )
    ).bind("funcTemplate");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}