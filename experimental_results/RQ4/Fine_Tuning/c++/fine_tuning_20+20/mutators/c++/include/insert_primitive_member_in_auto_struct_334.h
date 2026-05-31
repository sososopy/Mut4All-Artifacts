//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Primitive_Member_In_Auto_Struct_334
 */ 
class MutatorFrontendAction_334 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(334)

private:
    class MutatorASTConsumer_334 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_334(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_structs;
    };
};

