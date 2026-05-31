//header file
#pragma once
#include "Mutator_base.h"

/**
 * Export_Template_Within_Namespace_303
 */ 
class MutatorFrontendAction_303 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(303)

private:
    class MutatorASTConsumer_303 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_303(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_303.h"

// ========================================================================================================
#define MUT303_OUTPUT 1

void MutatorFrontendAction_303::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
            return;

        if (TD->isThisDeclarationADefinition()) {
            auto templateText = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
            if (templateText.find("template<>") != std::string::npos) {
                // Remove specialization
                size_t pos = templateText.find("template<>");
                if (pos != std::string::npos) {
                    templateText.erase(pos, 10);
                }
            }
            // Insert "export" keyword
            if (templateText.find("export") == std::string::npos) {
                size_t pos = templateText.find("template");
                if (pos != std::string::npos) {
                    templateText.insert(pos, "export ");
                }
            }
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateText);
        }
    }
}

void MutatorFrontendAction_303::MutatorASTConsumer_303::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(has(templateDecl().bind("TemplateDecl"))).bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}