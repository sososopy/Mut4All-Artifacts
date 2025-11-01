//header file
#pragma once
#include "Mutator_base.h"

/**
 * Static_Member_Function_With_Incomplete_Type_244
 */ 
class MutatorFrontendAction_244 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(244)

private:
    class MutatorASTConsumer_244 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_244(Rewriter &R) : TheRewriter(R) {}
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

