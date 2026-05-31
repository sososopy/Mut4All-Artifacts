//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceStaticAssertWithInvalidExpression_238
 */ 
class MutatorFrontendAction_238 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_238>(TheRewriter);
    }

private:
    class MutatorASTConsumer_238 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_238(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite, SourceManager &SourceMgr) : Rewrite(Rewrite), SourceMgr(SourceMgr) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        SourceManager &SourceMgr;
        //Necessary node information record used in the mutation process
    };
};

