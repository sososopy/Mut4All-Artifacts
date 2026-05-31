//header file
#pragma once
#include "Mutator_base.h"

/**
 * expand_invalid_template_pack_30
 */ 
class MutatorFrontendAction_30 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(30)

private:
    class MutatorASTConsumer_30 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_30(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/expand_invalid_template_pack_30.h"

// ========================================================================================================
#define MUT30_OUTPUT 1

void MutatorFrontendAction_30::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (TD->getTemplatedDecl()->hasBody()) {
            auto body = TD->getTemplatedDecl()->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());

            // Introduce invalid pack expansion
            std::string mutation = "\n/*mut30*/int arr[] = { (args + 1)... };";
            if (bodyText.find("{") != std::string::npos) {
                bodyText.insert(bodyText.find("{") + 1, mutation);
            }

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
        }
    }
}

void MutatorFrontendAction_30::MutatorASTConsumer_30::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateTypeParmDecl(anyOf(isVariadic(), anything())))).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}