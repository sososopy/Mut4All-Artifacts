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
        virtual void run(const MatchFinder::MatchResult &Result) override;
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
    if (auto *TD = Result.Nodes.getNodeAs<clang::Decl>("spec")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (auto *C = clang::dyn_cast<clang::ClassTemplateSpecializationDecl>(TD)) {
            if (C->getSpecializationKind() != clang::TSK_ExplicitSpecialization)
                return;
        } else if (auto *F = clang::dyn_cast<clang::FunctionTemplateSpecializationDecl>(TD)) {
            if (F->getSpecializationKind() != clang::TSK_ExplicitSpecialization)
                return;
        } else if (auto *V = clang::dyn_cast<clang::VarTemplateSpecializationDecl>(TD)) {
            if (V->getSpecializationKind() != clang::TSK_ExplicitSpecialization)
                return;
        } else {
            return;
        }

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
        clang::ast_matchers::templateSpecializationDecl(
            clang::ast_matchers::anyOf(
                clang::ast_matchers::isClass(),
                clang::ast_matchers::isFunction(),
                clang::ast_matchers::isVar()
            )
        ).bind("spec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}