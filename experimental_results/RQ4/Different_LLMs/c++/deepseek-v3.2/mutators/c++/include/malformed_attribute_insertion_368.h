//header file
#pragma once
#include "Mutator_base.h"

/**
 * Malformed_Attribute_Insertion_368
 */ 
class MutatorFrontendAction_368 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(368)

private:
    class MutatorASTConsumer_368 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_368(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
        std::vector<const clang::Decl *> declarations;
        Rewriter &Rewrite;
    };
};

