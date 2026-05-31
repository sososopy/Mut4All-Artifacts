//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Packed_Struct_With_Bitfield_And_Unsafe_Cast_312
 */ 
class MutatorFrontendAction_312 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(312)

private:
    class MutatorASTConsumer_312 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_312(Rewriter &R) : TheRewriter(R) {}
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
    };
};

