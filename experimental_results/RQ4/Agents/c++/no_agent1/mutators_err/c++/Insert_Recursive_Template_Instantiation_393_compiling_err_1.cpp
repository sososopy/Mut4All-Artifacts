//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_393
 */ 
class MutatorFrontendAction_393 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(393)

private:
    class MutatorASTConsumer_393 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_393(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> templateClasses;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_393.h"

// ========================================================================================================
#define MUT393_OUTPUT 1

void MutatorFrontendAction_393::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        if (!TD->isThisDeclarationADefinition())
            return;
        
        templateClasses.push_back(TD->getTemplatedDecl());
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        
        if (templateClasses.empty())
            return;

        const clang::CXXRecordDecl *templateClass = templateClasses.back();
        std::string className = templateClass->getNameAsString();
        std::string templateInstantiation = "/*mut393*/ " + className + "<" + className + "<int>> instance;\n";

        SourceLocation insertLoc = FD->getBody()->getBeginLoc().getLocWithOffset(1);
        Rewrite.InsertText(insertLoc, templateInstantiation);
    }
}

void MutatorFrontendAction_393::MutatorASTConsumer_393::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("TemplateDecl");
    DeclarationMatcher functionMatcher = functionDecl(hasBody()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}