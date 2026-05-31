//header file
#pragma once
#include "Mutator_base.h"

/**
 * NestedStructExportMutator_290
 */ 
class MutatorFrontendAction_290 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override;

private:
    class MutatorASTConsumer_290 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_290(clang::Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        clang::Rewriter &TheRewriter;
    
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(clang::Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
    private:
        clang::Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

