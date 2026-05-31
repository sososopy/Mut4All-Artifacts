//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_buffer_and_format_string_in_snprintf_539
 */ 
class MutatorFrontendAction_539 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(539)

private:
    class MutatorASTConsumer_539 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_539(Rewriter &R) : TheRewriter(R) {}
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

