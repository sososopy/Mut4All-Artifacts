//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Template_Constraint_Overload_432
 */ 
class MutatorFrontendAction_432 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(432)

private:
    class MutatorASTConsumer_432 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_432(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::CXXRecordDecl*> targetClasses;
    };
};

