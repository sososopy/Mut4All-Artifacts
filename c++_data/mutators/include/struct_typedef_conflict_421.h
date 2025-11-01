//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Typedef_Conflict_421
 */ 
class MutatorFrontendAction_421 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(421)

private:
    class MutatorASTConsumer_421 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_421(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> typedefNames;
    };
};

