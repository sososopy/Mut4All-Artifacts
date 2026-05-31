//header file
#pragma once
#include "Mutator_base.h"

/**
 * incomplete_class_definition_with_template_33
 */ 
class MutatorFrontendAction_33 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(33)

private:
    class MutatorASTConsumer_33 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_33(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> incompleteTemplates;
    };
};

//source file
#include "../include/incomplete_class_definition_with_template_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        
        if (TD->getTemplatedDecl()->isCompleteDefinition())
            return;

        incompleteTemplates.push_back(TD);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        for (const auto *TD : incompleteTemplates) {
            std::string templateName = TD->getNameAsString();
            std::string mutation = "/*mut33*/ " + templateName + "<int> obj; obj.someMemberFunction();\n";
            Rewrite.InsertTextBefore(FD->getBody()->getBeginLoc(), mutation);
        }
    }
}
  
void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("TemplateDecl");
    StatementMatcher functionMatcher = functionDecl(hasBody(compoundStmt())).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}