//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Global_Variable_Initialization_With_Different_Literal_Value_461
 */ 
class MutatorFrontendAction_461 : public MutatorFrontendAction {
public:
    MutatorFrontendAction_461() {}
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_461>(TheRewriter);
    }

private:
    class MutatorASTConsumer_461 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_461(Rewriter &R) : TheRewriter(R) {}
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

