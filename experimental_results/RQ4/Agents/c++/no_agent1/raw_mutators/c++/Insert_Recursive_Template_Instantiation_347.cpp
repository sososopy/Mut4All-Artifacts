//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_347
 */ 
class MutatorFrontendAction_347 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(347)

private:
    class MutatorASTConsumer_347 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_347(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> templateNames;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_347.h"

// ========================================================================================================
#define MUT347_OUTPUT 1

void MutatorFrontendAction_347::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        // Get the source code text of target node
        auto templateName = TD->getNameAsString();
        templateNames.push_back(templateName);

        // Perform mutation by adding a recursive instantiation
        std::string recursiveInstance = "template<> class " + templateName + "<0> { public: static const int value = " + templateName + "<1>::value; };";
        std::string recursiveInstance2 = "template<> class " + templateName + "<1> { public: static const int value = " + templateName + "<0>::value; };";
        std::string mutationText = "\n/*mut347*/" + recursiveInstance + "\n" + recursiveInstance2 + "\n";
        
        // Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(TD->getEndLoc(), mutationText);
    }
}

void MutatorFrontendAction_347::MutatorASTConsumer_347::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}