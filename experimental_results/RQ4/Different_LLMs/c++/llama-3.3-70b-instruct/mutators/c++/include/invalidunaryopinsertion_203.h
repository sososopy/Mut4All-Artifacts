```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * InvalidUnaryOpInsertion_203
 */ 
class MutatorFrontendAction_InvalidUnaryOpInsertion_203 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(InvalidUnaryOpInsertion_203)

private:
    class MutatorASTConsumer_InvalidUnaryOpInsertion_203 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_InvalidUnaryOpInsertion_203(Rewriter &R) : TheRewriter(R) {}
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

