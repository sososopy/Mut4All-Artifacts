//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_With_Auto_436
 */ 
class MutatorFrontendAction_436 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override;

private:
    class MutatorASTConsumer_436 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_436(clang::Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        clang::Rewriter &TheRewriter;
    
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(clang::Rewriter &Rewrite, clang::ASTContext &Context, clang::SourceManager &SourceManager) 
            : Rewrite(Rewrite), Context(Context), SourceManager(SourceManager) {}
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
    private:
        clang::Rewriter &Rewrite;
        clang::ASTContext &Context;
        clang::SourceManager &SourceManager;
        //Necessary node information record used in the mutation process
    };
};

