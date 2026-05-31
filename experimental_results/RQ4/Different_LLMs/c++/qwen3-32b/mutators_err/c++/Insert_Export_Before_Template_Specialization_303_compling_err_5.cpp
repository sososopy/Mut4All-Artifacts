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
#include "clang/AST/DeclTemplate.h"
#include "clang/ASTMatchers/ASTMatchers.h"

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
            SourceLocation templateLoc = C->getTemplateKeywordLoc();
            if (templateLoc.isInvalid())
                return;
            Rewrite.InsertTextBefore(templateLoc, "export ");
        } else if (auto *F = clang::dyn_cast<clang::FunctionDecl>(TD)) {
            if (F->getTemplateSpecializationKind() != clang::TSK_ExplicitSpecialization)
                return;
            SourceLocation templateLoc = F->getTemplateKeywordLoc();
            if (templateLoc.isInvalid())
                return;
            Rewrite.InsertTextBefore(templateLoc, "export ");
        } else if (auto *V = clang::dyn_cast<clang::VarDecl>(TD)) {
            if (V->getTemplateSpecializationKind() != clang::TSK_ExplicitSpecialization)
                return;
            SourceLocation templateLoc = V->getTemplateKeywordLoc();
            if (templateLoc.isInvalid())
                return;
            Rewrite.InsertTextBefore(templateLoc, "export ");
        } else {
            return;
        }
    }
}
  
void MutatorFrontendAction_303::MutatorASTConsumer_303::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher matcher = 
        clang::ast_matchers::classTemplateSpecializationDecl().bind("spec") ||
        clang::ast_matchers::functionDecl(clang::ast_matchers::isExplicitSpecialization()).bind("spec") ||
        clang::ast_matchers::varDecl(clang::ast_matchers::isExplicitSpecialization()).bind("spec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}