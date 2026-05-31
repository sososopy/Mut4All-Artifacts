//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_Forward_Declaration_in_Template_Metaprogramming_231
 */ 
class MutatorFrontendAction_231 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(231)

private:
    class MutatorASTConsumer_231 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_231(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

