//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Specialization_With_Primary_Template_25
 */ 
class MutatorFrontendAction_25 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_25>(TheRewriter);
    }

private:
    class MutatorASTConsumer_25 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_25(clang::Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override {}
    private:
        clang::Rewriter &TheRewriter;
    
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(clang::Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override {}
    private:
        clang::Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
    clang::Rewriter TheRewriter;
};

