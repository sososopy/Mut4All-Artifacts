//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misplace_Template_Specialization_398
 */ 
class MutatorFrontendAction_398 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(398)

private:
    class MutatorASTConsumer_398 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_398(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl*> specializations;
    };
};

