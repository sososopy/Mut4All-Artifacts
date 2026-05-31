//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_With_Templated_Fold_Expression_132
 */ 
class MutatorFrontendAction_132 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(132)

private:
    class MutatorASTConsumer_132 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_132(Rewriter &R) : TheRewriter(R) {}
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

