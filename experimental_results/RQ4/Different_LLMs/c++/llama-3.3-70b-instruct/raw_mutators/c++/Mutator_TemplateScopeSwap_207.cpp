//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateScopeSwap_207
 */ 
class MutatorFrontendAction_TemplateScopeSwap_207 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(207)

private:
    class MutatorASTConsumer_TemplateScopeSwap_207 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_TemplateScopeSwap_207(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl *> templateDecls;
    };
};

//source file
#include "../include/Mutator_TemplateScopeSwap_207.h"

// ========================================================================================================
#define MUT207_OUTPUT 1

void MutatorFrontendAction_TemplateScopeSwap_207::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record the template declaration
      templateDecls.push_back(TD);
    }
    // Perform scope swap when we have at least two template declarations
    if (templateDecls.size() >= 2) {
        // Get the namespaces of the two template declarations
        auto namespace1 = templateDecls[0]->getNamespace();
        auto namespace2 = templateDecls[1]->getNamespace();
        // Swap the namespaces
        templateDecls[0]->setNamespace(namespace2);
        templateDecls[1]->setNamespace(namespace1);
        // Replace the original AST nodes with the mutated ones
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(templateDecls[0]->getSourceRange()), templateDecls[0]->getDeclAsText());
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(templateDecls[1]->getSourceRange()), templateDecls[1]->getDeclAsText());
    }
}

void MutatorFrontendAction_TemplateScopeSwap_207::MutatorASTConsumer_TemplateScopeSwap_207::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}