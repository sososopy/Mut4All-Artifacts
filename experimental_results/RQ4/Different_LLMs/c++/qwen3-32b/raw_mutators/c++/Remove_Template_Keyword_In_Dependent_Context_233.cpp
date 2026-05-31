//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Template_Keyword_In_Dependent_Context_233
 */ 
class MutatorFrontendAction_Remove_Template_Keyword_In_Dependent_Context_233 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Remove_Template_Keyword_In_Dependent_Context_233)

private:
    class MutatorASTConsumer_Remove_Template_Keyword_In_Dependent_Context_233 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Remove_Template_Keyword_In_Dependent_Context_233(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Template_Keyword_In_Dependent_Context_233.h"

// ========================================================================================================
#define MUTRemove_Template_Keyword_In_Dependent_Context_233_OUTPUT 1

void MutatorFrontendAction_Remove_Template_Keyword_In_Dependent_Context_233::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXDependentScopeMemberExpr>("template_member")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        if (MT->getTemplateKeywordLoc().isInvalid())
            return;
        Rewrite.RemoveText(MT->getTemplateKeywordLoc(), 8);
    }
}
  
void MutatorFrontendAction_Remove_Template_Keyword_In_Dependent_Context_233::MutatorASTConsumer_Remove_Template_Keyword_In_Dependent_Context_233::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxDependentScopeMemberExpr().bind("template_member");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}