//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Void_Call_With_Mismatched_Assignment_176
 */ 
class MutatorFrontendAction_176 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(176)

private:
    class MutatorASTConsumer_176 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_176(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableTypes;
    };
};

