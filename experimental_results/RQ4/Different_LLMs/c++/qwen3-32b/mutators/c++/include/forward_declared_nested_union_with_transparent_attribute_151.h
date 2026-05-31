//header file
#pragma once
#include "Mutator_base.h"

/**
 * Forward_Declared_Nested_Union_With_Transparent_Attribute_151
 */ 
class MutatorFrontendAction_151 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(151)

private:
    class MutatorASTConsumer_151 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_151(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

