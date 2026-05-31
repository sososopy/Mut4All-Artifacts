//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_412
 */ 
class MutatorFrontendAction_412 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(412)

private:
    class MutatorASTConsumer_412 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_412(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> visitedTemplates;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_412.h"

// ========================================================================================================
#define MUT412_OUTPUT 1

void MutatorFrontendAction_412::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TD->getLocation()))
            return;

        std::string templateName = TD->getNameAsString();
        if (visitedTemplates.find(templateName) != visitedTemplates.end())
            return;

        visitedTemplates.insert(templateName);

        // Perform mutation to create recursive template instantiation
        std::string instantiation = "template<> class " + templateName + "<0> { public: static const int value = " + templateName + "<1>::value; };";
        std::string insertion = "/*mut412*/" + instantiation + "\n";

        // Insert the recursive instantiation after the template declaration
        Rewrite.InsertTextAfterToken(TD->getEndLoc(), insertion);
    }
}

void MutatorFrontendAction_412::MutatorASTConsumer_412::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}