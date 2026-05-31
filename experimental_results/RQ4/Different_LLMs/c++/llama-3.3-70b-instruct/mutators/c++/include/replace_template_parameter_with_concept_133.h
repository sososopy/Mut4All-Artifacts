//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * Replace_Template_Parameter_With_Concept_133
 */ 
class MutatorFrontendAction_133 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(133)

private:
    class MutatorASTConsumer_133 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_133(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite, ASTContext *Context, clang::SourceManager *SourceMgr) 
            : Rewrite(Rewrite), Context(Context), SourceMgr(SourceMgr) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        ASTContext *Context;
        clang::SourceManager *SourceMgr;
        //Necessary node information record used in the mutation process
    };
};

