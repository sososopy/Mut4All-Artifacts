//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Parenthesized_Initializer_List_For_Complex_Typing_111
 */ 
class MutatorFrontendAction_111 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(111)

private:
    class MutatorASTConsumer_111 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_111(Rewriter &R) : TheRewriter(R) {}
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
    };
};

