//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Enum_Cross_Mutation_320
 */ 
class MutatorFrontendAction_320 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(320)

private:
    class MutatorASTConsumer_320 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_320(Rewriter &R) : TheRewriter(R) {}
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

