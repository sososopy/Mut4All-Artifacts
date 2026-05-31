//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_parameter_mismatch_503
 */ 
class MutatorFrontendAction_503 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(503)

private:
    class MutatorASTConsumer_503 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_503(Rewriter &R) : TheRewriter(R) {}
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
        std::map<std::string, std::string> templateTypeMap;
    };
};

