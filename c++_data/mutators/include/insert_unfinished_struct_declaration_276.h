//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unfinished_Struct_Declaration_276
 */ 
class MutatorFrontendAction_276 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(276)

private:
    class MutatorASTConsumer_276 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_276(Rewriter &R) : TheRewriter(R) {}
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

