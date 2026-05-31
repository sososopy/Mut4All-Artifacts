//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_374
 */ 
class MutatorFrontendAction_374 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(374)

private:
    class MutatorASTConsumer_374 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_374(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_recursive_template_instantiation_374.h"

// ========================================================================================================
#define MUT374_OUTPUT 1

void MutatorFrontendAction_374::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (!TD->isThisDeclarationADefinition())
            return;

        auto templateName = TD->getNameAsString();
        auto templateParams = TD->getTemplateParameters();
        if (templateParams->size() == 0)
            return;

        auto paramName = templateParams->getParam(0)->getNameAsString();
        std::string recursiveInst = "template<> struct " + templateName + "<0> { using type = " + templateName + "<1>; };";
        std::string recursiveInstCommented = "/*mut374*/" + recursiveInst;

        Rewrite.InsertTextAfterToken(TD->getEndLoc(), "\n" + recursiveInstCommented);
    }
}

void MutatorFrontendAction_374::MutatorASTConsumer_374::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}