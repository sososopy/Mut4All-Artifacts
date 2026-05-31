//header file
#pragma once
#include "Mutator_base.h"

/**
 * Rename_Enum_Constant_67
 */ 
class MutatorFrontendAction_67 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(67)

private:
    class MutatorASTConsumer_67 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_67(Rewriter &R) : TheRewriter(R) {}
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

