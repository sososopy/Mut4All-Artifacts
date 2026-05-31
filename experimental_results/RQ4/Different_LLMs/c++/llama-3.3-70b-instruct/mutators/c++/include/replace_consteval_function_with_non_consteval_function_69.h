//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Consteval_Function_With_Non_Consteval_Function_69
 */ 
class MutatorFrontendAction_69 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_69>(TheRewriter);
    }

private:
    class MutatorASTConsumer_69 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_69(Rewriter &R) : TheRewriter(R) {}
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

