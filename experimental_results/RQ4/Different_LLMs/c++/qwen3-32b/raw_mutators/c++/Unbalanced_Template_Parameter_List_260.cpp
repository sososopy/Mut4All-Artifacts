//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/DeclTemplate.h"

/**
 * Unbalanced_Template_Parameter_List_260
 */ 
class MutatorFrontendAction_260 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(260)

private:
    class MutatorASTConsumer_260 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_260(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Unbalanced_Template_Parameter_List_260.h"

// ========================================================================================================
#define MUT260_OUTPUT 1

void MutatorFrontendAction_260::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateName")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;

        auto *TPL = FTD->getTemplateParameters();
        if (!TPL)
            return;

        SourceLocation endLoc = TPL->getSourceRange().getEnd();
        Rewrite.ReplaceText(endLoc, 1, "<");
    }
}
  
void MutatorFrontendAction_260::MutatorASTConsumer_260::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("TemplateName");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}