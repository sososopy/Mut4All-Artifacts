//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * Replace_Requires_Clause_482
 */ 
class MutatorFrontendAction_482 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_482>(TheRewriter);
    }

private:
    Rewriter TheRewriter;
    class MutatorASTConsumer_482 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_482(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Requires_Clause_482.h"
#include "clang/AST/Type.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT482_OUTPUT 1

void MutatorFrontendAction_482::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RC = Result.Nodes.getNodeAs<clang::CXXRequiresExpr>("RequiresClause")) {
      //Filter nodes in header files
      if (!RC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RC->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(RC->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      // Replace the original requires clause with a new one
      std::string newRequiresClause = "requires { typename T::y; }";
      declaration.replace(declaration.find("requires"), declaration.find("}"), newRequiresClause);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(RC->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_482::MutatorASTConsumer_482::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRequiresExpr().bind("RequiresClause");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}