//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Template_Parameter_List_At_EOF_283
 */ 
class MutatorFrontendAction_283 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(283)

private:
    class MutatorASTConsumer_283 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_283(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Mutator_Incomplete_Template_Parameter_List_At_EOF_283.h"

// ========================================================================================================
#define MUT283_OUTPUT 1

void MutatorFrontendAction_283::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("templateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        const auto *TPL = TD->getTemplateParameters();
        if (!TPL)
            return;
        auto range = TPL->getSourceRange();
        if (range.getBegin().isInvalid() || range.getEnd().isInvalid())
            return;
        Rewrite.RemoveText(range.getEnd(), 1);
    }
}
  
void MutatorFrontendAction_283::MutatorASTConsumer_283::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::functionTemplateDecl().bind("templateDecl") || 
                                 clang::ast_matchers::classTemplateDecl().bind("templateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}