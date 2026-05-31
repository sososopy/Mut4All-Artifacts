//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Template_Keyword_In_Dependent_Types_293
 */ 
class MutatorFrontendAction_293 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(293)

private:
    class MutatorASTConsumer_293 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_293(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Template_Keyword_In_Dependent_Types_293.h"

// ========================================================================================================
#define MUT293_OUTPUT 1

void MutatorFrontendAction_293::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TID = Result.Nodes.getNodeAs<clang::TemplateIdExpr>("templateId")) {
        if (!TID || !Result.Context->getSourceManager().isWrittenInMainFile(TID->getBeginLoc()))
            return;
        
        const NestedNameSpecifier *Qualifier = TID->getQualifier();
        if (!Qualifier || !Qualifier->hasTemplateKeyword())
            return;
        
        SourceLocation TemplateLoc = Qualifier->getTemplateKeywordLoc();
        if (TemplateLoc.isInvalid())
            return;
        
        Rewrite.RemoveText(TemplateLoc, strlen("template"));
    }
}

void MutatorFrontendAction_293::MutatorASTConsumer_293::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateIdExpr(hasQualifier(nestedNameSpecifier(hasTemplateKeyword()))).bind("templateId");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}