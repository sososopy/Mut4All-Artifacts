//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Spurious_Identifier_Before_Constructor_354
 */ 
class MutatorFrontendAction_354 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(354)

private:
    class MutatorASTConsumer_354 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_354(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> identifierPool = {"temp", "label", "data", "value", "obj"};
    };
};

