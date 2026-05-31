//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constrained_Auto_With_Template_103
 */ 
class MutatorFrontendAction_103 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(103)

private:
    class MutatorASTConsumer_103 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_103(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback