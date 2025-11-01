//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_With_Forward_Declaration_435
 */ 
class MutatorFrontendAction_435 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(435)

private:
    class MutatorASTConsumer_435 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_435(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::EnumDecl *> forwardDeclaredEnums;
    };
};

