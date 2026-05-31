//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_annotations_in_template_classes_537
 */ 
class MutatorFrontendAction_537 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(537)

private:
    class MutatorASTConsumer_537 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_537(Rewriter &R) : TheRewriter(R) {}
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
    };
};

