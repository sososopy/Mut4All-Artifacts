//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Complex_Template_Instantiation_113
 */ 
class MutatorFrontendAction_113 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(113)

private:
    class MutatorASTConsumer_113 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_113(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> template_classes;
    };
};

//source file
#include "../include/insert_complex_template_instantiation_113.h"

// ========================================================================================================
#define MUT113_OUTPUT 1

void MutatorFrontendAction_113::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TD->getLocation()))
            return;
        if (!TD->isThisDeclarationADefinition())
            return;
        
        template_classes.push_back(TD->getTemplatedDecl());

        // Get the source code text of the target node
        auto templateName = TD->getNameAsString();
        std::string instantiation = "template class " + templateName + "<int, double>;\n";

        // Perform mutation on the source code text by applying string replacement
        instantiation = "/*mut113*/" + instantiation;

        // Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(TD->getEndLoc(), instantiation);
    }
}

void MutatorFrontendAction_113::MutatorASTConsumer_113::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}