//header file
#pragma once
#include "Mutator_base.h"

/**
 * Default_Argument_with_Initialization_in_Array_or_Function_Call_Context_167
 */ 
class MutatorFrontendAction_167 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(167)

private:
    class MutatorASTConsumer_167 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_167(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> cur_functions;
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

