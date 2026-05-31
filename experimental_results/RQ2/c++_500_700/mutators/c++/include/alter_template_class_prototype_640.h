//header file
#pragma once
#include "Mutator_base.h"
#include <vector>
#include <string>

/**
 * alter_template_class_prototype_640
 */ 
class MutatorFrontendAction_640 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(640)

private:
    class MutatorASTConsumer_640 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_640(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> existingClassNames;
    };
};

