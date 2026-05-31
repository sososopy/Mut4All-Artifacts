//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Template_Parameter_List_Closure_202
 */ 
class MutatorFrontendAction_202 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(202)

private:
    class MutatorASTConsumer_202 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_202(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalid_Template_Parameter_List_Closure_202.h"

// ========================================================================================================
#define MUT202_OUTPUT 1

void MutatorFrontendAction_202::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("templateDecl")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        if (auto paramList = MT->getTemplateParameters()) {
            SourceLocation endLoc = paramList->getSourceRange().getEnd();
            Rewrite.ReplaceText(endLoc, 1, "}");
        }
    }
}
  
void MutatorFrontendAction_202::MutatorASTConsumer_202::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateDecl(isMemberOfRecord(cxxRecordDecl())).bind("templateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}