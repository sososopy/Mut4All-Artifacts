//header file
#pragma once
#include "Mutator_base.h"
#include <regex>
#include <string>

/**
 * Modify_Enum_Usage_83
 */ 
class MutatorFrontendAction_83 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(83)

private:
    class MutatorASTConsumer_83 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_83(Rewriter &R) : TheRewriter(R) {}
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

