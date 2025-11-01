//header file
#pragma once
#include "Mutator_base.h"

/**
 * Forward_Declared_Enum_With_Type_Traits_424
 */ 
class MutatorFrontendAction_424 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(424)

private:
    class MutatorASTConsumer_424 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_424(Rewriter &R) : TheRewriter(R) {}
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

