//header file
#pragma once
#include "Mutator_base.h"

/**
 * introduce_template_member_pointer_565
 */ 
class MutatorFrontendAction_565 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(565)

private:
    class MutatorASTConsumer_565 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_565(Rewriter &R) : TheRewriter(R) {}
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

