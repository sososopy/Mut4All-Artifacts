//header file
#pragma once
#include "Mutator_base.h"

/**
 * relocate_template_deduction_guide_12
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
        const ClassTemplateDecl *templateDecl = nullptr;
    };
};

//source file
#include "../include/relocate_template_deduction_guide_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check if the matched AST node is the target node
    if (auto *NDG = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("DeductionGuide")) {
        // Filter nodes in header files
        if (!NDG || !Result.Context->getSourceManager().isWrittenInMainFile(NDG->getLocation()))
            return;

        // Get the source code text of the target node
        auto deductionGuideText = stringutils::rangetoStr(*(Result.SourceManager), NDG->getSourceRange());

        // Perform mutation by relocating the deduction guide outside the namespace
        if (currentNamespace && templateDecl) {
            std::string relocationText = "\n/*mut12*/" + deductionGuideText;
            Rewrite.InsertTextAfterToken(currentNamespace->getRBraceLoc(), relocationText);
            Rewrite.RemoveText(NDG->getSourceRange());
        }
    } else if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
        currentNamespace = NS;
    } else if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
        templateDecl = CTD;
    }
}

void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    auto deductionGuideMatcher = classTemplateSpecializationDecl().bind("DeductionGuide");
    auto namespaceMatcher = namespaceDecl().bind("Namespace");
    auto templateMatcher = classTemplateDecl().bind("Template");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(deductionGuideMatcher, &callback);
    matchFinder.addMatcher(namespaceMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}