//header file
#pragma once
#include "Mutator_base.h"

/**
 * Defaulted_Assignment_Operator_With_Explicit_Object_Parameter_In_Derived_Struct_351
 */ 
class MutatorFrontendAction_351 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(351)

private:
    class MutatorASTConsumer_351 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_351(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> base_structs;
    };
};

