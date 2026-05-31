//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_array_access_in_constexpr_516
 */ 
class MutatorFrontendAction_516 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(516)

private:
    class MutatorASTConsumer_516 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_516(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const clang::VarDecl *, unsigned> arraySizes;
    };
};

