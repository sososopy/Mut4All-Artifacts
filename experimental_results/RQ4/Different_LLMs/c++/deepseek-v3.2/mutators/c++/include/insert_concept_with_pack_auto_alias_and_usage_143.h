//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Concept_With_Pack_Auto_Alias_And_Usage_143
 */ 
class MutatorFrontendAction_143 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(143)

private:
    class MutatorASTConsumer_143 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_143(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::NamespaceDecl *> targetNamespaces;
    };
};

