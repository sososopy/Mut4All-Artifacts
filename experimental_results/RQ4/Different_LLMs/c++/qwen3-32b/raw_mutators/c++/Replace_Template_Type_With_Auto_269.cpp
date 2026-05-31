//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Type_With_Auto_269
 */ 
class MutatorFrontendAction_269 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(269)

private:
    class MutatorASTConsumer_269 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_269(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Type_With_Auto_269.h"

// ========================================================================================================
#define MUT269_OUTPUT 1

void MutatorFrontendAction_269::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TParam = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("tparam")) {
        if (!TParam || !Result.Context->getSourceManager().isWrittenInMainFile(TParam->getLocation()))
            return;
        
        SourceRange range = TParam->getSourceRange();
        std::string replacement = "auto " + TParam->getNameAsString();
        Rewrite.ReplaceText(range, replacement);
    }
}
  
void MutatorFrontendAction_269::MutatorASTConsumer_269::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateTypeParmDecl().bind("tparam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}