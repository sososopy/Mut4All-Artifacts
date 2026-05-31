//header file
#pragma once
#include "Mutator_base.h"

/**
 * replace_enum_base_type_in_templated_struct_536
 */ 
class MutatorFrontendAction_536 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(536)

private:
    class MutatorASTConsumer_536 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_536(Rewriter &R) : TheRewriter(R) {}
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

