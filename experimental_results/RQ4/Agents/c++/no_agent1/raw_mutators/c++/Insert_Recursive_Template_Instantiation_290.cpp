//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_290
 */ 
class MutatorFrontendAction_290 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(290)

private:
    class MutatorASTConsumer_290 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_290(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::ClassTemplateDecl *> visitedTemplates;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_290.h"

// ========================================================================================================
#define MUT290_OUTPUT 1

void MutatorFrontendAction_290::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                CTD->getLocation()))
            return;

        if (visitedTemplates.find(CTD) != visitedTemplates.end())
            return;

        visitedTemplates.insert(CTD);

        // Get the source code text of target node
        auto templateName = CTD->getNameAsString();
        std::string recursiveInstantiation = "template<> struct " + templateName + "<0> { using type = " + templateName + "<0>; };";

        // Perform mutation on the source code text by applying string replacement
        std::string insertion = "\n/*mut290*/" + recursiveInstantiation + "\n";
        
        // Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(CTD->getEndLoc(), insertion);
    }
}

void MutatorFrontendAction_290::MutatorASTConsumer_290::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}