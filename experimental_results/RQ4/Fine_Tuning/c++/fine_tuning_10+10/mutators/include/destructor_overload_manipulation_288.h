//header file
#pragma once
#include "Mutator_base.h"

/**
 * Destructor_Overload_Manipulation_288
 */ 
class MutatorFrontendAction_288 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(288)

private:
    class MutatorASTConsumer_288 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_288(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const CXXRecordDecl*> processedClasses;
    };
};

