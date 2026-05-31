//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_312
 */ 
class MutatorFrontendAction_312 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(312)

private:
    class MutatorASTConsumer_312 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_312(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_recursive_template_instantiation_312.h"

// ========================================================================================================
#define MUT312_OUTPUT 1

void MutatorFrontendAction_312::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        auto templateName = TD->getNameAsString();
        if (visitedTemplates.find(templateName) != visitedTemplates.end())
            return;

        visitedTemplates.insert(templateName);

        // Get the source code text of target node
        auto templateSource = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());

        // Perform mutation by creating a recursive instantiation
        std::string recursiveInstantiation = "template<> struct " + templateName + "<0> { static const int value = " + templateName + "<1>::value; };\n";
        recursiveInstantiation = "/*mut312*/\n" + recursiveInstantiation;

        // Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(TD->getEndLoc(), recursiveInstantiation);
    }
}

void MutatorFrontendAction_312::MutatorASTConsumer_312::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}