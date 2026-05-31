//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_175
 */ 
class MutatorFrontendAction_175 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(175)

private:
    class MutatorASTConsumer_175 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_175(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_recursive_template_instantiation_175.h"

// ========================================================================================================
#define MUT175_OUTPUT 1

void MutatorFrontendAction_175::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        // Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TD->getLocation()))
            return;

        // Get the source code text of target node
        auto templateName = TD->getNameAsString();
        if (templateName.empty())
            return;

        // Perform mutation by inserting a recursive template instantiation
        std::string recursiveTemplate = "\n/*mut175*/template<int N> struct " + templateName + " { " +
                                        "static const int value = " + templateName + "<N-1>::value; };" +
                                        "template<> struct " + templateName + "<0> { static const int value = 0; };";

        // Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(TD->getEndLoc(), recursiveTemplate);
    }
}
  
void MutatorFrontendAction_175::MutatorASTConsumer_175::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}