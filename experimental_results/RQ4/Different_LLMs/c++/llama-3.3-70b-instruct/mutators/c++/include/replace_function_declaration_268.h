//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Declaration_268
 */ 
class MutatorFrontendAction_268 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_268>(TheRewriter);
    }

private:
    class MutatorASTConsumer_268 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_268(Rewriter &R) : TheRewriter(R) {}
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
    Rewriter TheRewriter;
};

