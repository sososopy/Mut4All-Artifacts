//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Default_Arguments_In_Nested_Namespaces_230
 */ 
class MutatorFrontendAction_230 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(230)

private:
    class MutatorASTConsumer_230 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_230(Rewriter &R) : TheRewriter(R) {}
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
        std::string newNamespaceName;
        std::string newTypeName;
    };
};

