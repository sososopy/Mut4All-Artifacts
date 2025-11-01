//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Specialization_Constructor_Mutation_367
 */ 
class MutatorFrontendAction_367 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(367)

private:
    class MutatorASTConsumer_367 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_367(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const clang::CXXRecordDecl*, std::string> classNames;
    };
};

