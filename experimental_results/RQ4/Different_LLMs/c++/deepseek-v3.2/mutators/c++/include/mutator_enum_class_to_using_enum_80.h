//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Enum_Class_To_Using_Enum_80
 */ 
class MutatorFrontendAction_80 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(80)

private:
    class MutatorASTConsumer_80 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_80(Rewriter &R) : TheRewriter(R) {}
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

