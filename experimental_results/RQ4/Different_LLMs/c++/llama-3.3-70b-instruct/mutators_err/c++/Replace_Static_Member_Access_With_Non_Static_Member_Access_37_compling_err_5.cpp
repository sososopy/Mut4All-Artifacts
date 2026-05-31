//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/Support/raw_ostream.h"

/**
 * Replace_Static_Member_Access_With_Non_Static_Member_Access_37
 */ 
class MutatorFrontendAction_37 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_37>(TheRewriter);
    }

private:
    class MutatorASTConsumer_37 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_37(clang::Rewriter &R) : TheRewriter(R) {}
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
    clang::Rewriter TheRewriter;
};

//source file
#include "../include/Mutator_Replace_Static_Member_Access_With_Non_Static_Member_Access_37.h"

void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(clang::ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder Finder;
    Callback Handler(TheRewriter);
    Finder.addMatcher(memberExpr(hasObjectExpression(isa(cxxRecordDecl(has(name("::std::vector"))))).bind("vector"), &Handler);
    Finder.matchAST(Context);
}

void MutatorFrontendAction_37::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    if (const auto *ME = Result.Nodes.getNodeAs<clang::MemberExpr>("vector")) {
        // Perform the actual replacement here
        // TheRewriter.ReplaceText(ME->getLocStart(), ME->getLocEnd(), "replacement_text");
    }
}