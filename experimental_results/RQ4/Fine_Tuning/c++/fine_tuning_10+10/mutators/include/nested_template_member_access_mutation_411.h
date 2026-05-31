//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Template_Member_Access_Mutation_411
 */ 
class MutatorFrontendAction_411 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(411)

private:
    class MutatorASTConsumer_411 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_411(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const CXXRecordDecl*, std::string> nestedTemplateDeclarations;
    };
};

