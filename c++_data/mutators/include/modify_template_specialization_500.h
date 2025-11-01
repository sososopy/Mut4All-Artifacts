//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Specialization_500
 */ 
class MutatorFrontendAction_500 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(500)

private:
    class MutatorASTConsumer_500 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_500(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionDecl *SpecializedFunction = nullptr;
    };
};

