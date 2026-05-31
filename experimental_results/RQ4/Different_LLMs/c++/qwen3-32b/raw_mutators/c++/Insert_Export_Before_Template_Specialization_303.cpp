//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Export_Before_Template_Specialization_303
 */ 

class MutatorFrontendAction_303 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(303)

private:
    class MutatorASTConsumer_303 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_303(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Insert_Export_Before_Template_Specialization_303.h"

// ========================================================================================================
#define MUT303_OUTPUT 1

void MutatorFrontendAction_303::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("spec")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (TD->getSpecializationKind() != clang::TemplateDecl::TSK_ExplicitSpecialization)
            return;

        SourceLocation templateLoc = TD->getTemplateKeywordLoc();
        if (templateLoc.isInvalid())
            return;

        //Insert 'export' before the 'template' keyword
        Rewrite.InsertTextBefore(templateLoc, "export ");
    }
}
  
void MutatorFrontendAction_303::MutatorASTConsumer_303::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher matcher = 
        (varTemplateSpecializationDecl() |
         functionTemplateSpecializationDecl() |
         classTemplateSpecializationDecl())
        .bind("spec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}