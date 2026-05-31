//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Malformed_Attribute_On_Function_Declaration_368
 */ 
class MutatorFrontendAction_368 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(368)

private:
    class MutatorASTConsumer_368 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_368(Rewriter &R) : TheRewriter(R) {}
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

