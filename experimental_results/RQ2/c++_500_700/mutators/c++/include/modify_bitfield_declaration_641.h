//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_bitfield_declaration_641
 */ 
class MutatorFrontendAction_641 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(641)

private:
    class MutatorASTConsumer_641 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_641(Rewriter &R) : TheRewriter(R) {}
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

