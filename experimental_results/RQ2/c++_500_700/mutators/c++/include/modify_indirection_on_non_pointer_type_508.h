//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_indirection_on_non_pointer_type_508
 */ 
class MutatorFrontendAction_508 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(508)

private:
    class MutatorASTConsumer_508 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_508(Rewriter &R) : TheRewriter(R) {}
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
        const VarDecl *existingNonPointerVar = nullptr;
    };
};

