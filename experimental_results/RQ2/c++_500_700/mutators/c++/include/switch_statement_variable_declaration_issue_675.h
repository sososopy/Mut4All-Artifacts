//header file
#pragma once
#include "Mutator_base.h"

/**
 * switch_statement_variable_declaration_issue_675
 */ 
class MutatorFrontendAction_675 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(675)

private:
    class MutatorASTConsumer_675 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_675(Rewriter &R) : TheRewriter(R) {}
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
        std::string switchVarName;
    };
};

