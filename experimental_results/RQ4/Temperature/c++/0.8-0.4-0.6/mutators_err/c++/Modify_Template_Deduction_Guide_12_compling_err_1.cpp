//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_deduction_guide_12
 */ 
class MutatorFrontendAction_12 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(12)

private:
    class MutatorASTConsumer_12 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_12(Rewriter &R) : TheRewriter(R) {}
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
        const NamespaceDecl *currentNamespace = nullptr;
    };
};

//source file
#include "../include/modify_template_deduction_guide_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TDG = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateDeductionGuide")) {
        if (!TDG || !Result.Context->getSourceManager().isWrittenInMainFile(TDG->getLocation()))
            return;

        auto *associatedTemplate = TDG->getAssociatedFunction();
        if (!associatedTemplate)
            return;

        if (const auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
            currentNamespace = NS;
        }

        std::string nsPrefix = currentNamespace ? currentNamespace->getNameAsString() + "::" : "";

        std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), TDG->getSourceRange());
        std::string modifiedText = "template <class U> " + nsPrefix + associatedTemplate->getNameAsString() + "() -> " + nsPrefix + associatedTemplate->getNameAsString() + "<U>;";

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TDG->getSourceRange()), modifiedText);
    }
}

void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl(hasAncestor(namespaceDecl().bind("Namespace"))).bind("TemplateDeductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}