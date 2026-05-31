//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Invalid_Lambda_Capture_Transformation_104
 */ 
class MutatorFrontendAction_104 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(104)

private:
    class MutatorASTConsumer_104 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_104(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::LambdaExpr *> cur_lambdas;
    };
};

