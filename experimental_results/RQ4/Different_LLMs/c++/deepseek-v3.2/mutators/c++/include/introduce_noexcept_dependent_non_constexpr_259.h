//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Noexcept_Dependent_Non_Constexpr_259
 */ 
class MutatorFrontendAction_259 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(259)

private:
    class MutatorASTConsumer_259 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_259(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> candidateFunctions;
        std::vector<const clang::CXXRecordDecl*> candidateClasses;
    };
};

