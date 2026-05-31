//header file
#pragma once
#include "Mutator_base.h"

/**
 * Apply_Address_Space_Attribute_To_Pointer_Member_392
 */ 
class MutatorFrontendAction_392 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(392)

private:
    class MutatorASTConsumer_392 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_392(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

