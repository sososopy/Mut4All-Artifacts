//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/Type.h"

/**
 * Add_Variadic_Using_Declaration_With_Virtual_Inheritance_467
 */ 
class MutatorFrontendAction_467 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(467)
private:
    class MutatorASTConsumer_467 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_467(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

