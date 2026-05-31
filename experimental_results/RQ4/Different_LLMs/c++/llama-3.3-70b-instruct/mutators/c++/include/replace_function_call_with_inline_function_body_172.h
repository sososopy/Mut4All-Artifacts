//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/DeclCXX.h" // Include necessary header for CXXDestructor
#include "clang/AST/ASTContext.h" // Include necessary header for dyn_cast

/**
 * Replace_Function_Call_With_Inline_Function_Body_172
 */ 
class MutatorFrontendAction_172 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_172>(TheRewriter);
    }

private:
    class MutatorASTConsumer_172 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_172(Rewriter &R) : TheRewriter(R) {}
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

