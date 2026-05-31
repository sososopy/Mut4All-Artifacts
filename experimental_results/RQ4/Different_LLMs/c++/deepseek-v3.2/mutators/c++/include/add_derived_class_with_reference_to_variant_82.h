//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Derived_Class_With_Reference_To_Variant_82
 */ 
class MutatorFrontendAction_82 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(82)

private:
    class MutatorASTConsumer_82 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_82(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : TheRewriter(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &TheRewriter;
        //Necessary node information record used in the mutation process
        std::vector<const clang::Type*> availableTypes;
        std::vector<const clang::CXXRecordDecl*> baseClasses;
    };
};

