//header file
#pragma once
#include "Mutator_base.h"

/**
 * target_template_class_unqualified_lookup_580
 */ 
class MutatorFrontendAction_580 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(580)

private:
    class MutatorASTConsumer_580 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_580(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> templateClasses;
    };
};

