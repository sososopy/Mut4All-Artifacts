//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_Switch_420
 */ 
class MutatorFrontendAction_420 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(420)

private:
    class MutatorASTConsumer_420 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_420(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::EnumDecl *> cur_enums;
    };
};

