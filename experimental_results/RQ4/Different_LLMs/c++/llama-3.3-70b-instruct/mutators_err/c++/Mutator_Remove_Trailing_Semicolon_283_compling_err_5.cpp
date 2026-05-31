//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Trailing_Semicolon_283
 */ 
class MutatorFrontendAction_Remove_Trailing_Semicolon_283 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Remove_Trailing_Semicolon_283)

private:
    class MutatorASTConsumer_Remove_Trailing_Semicolon_283 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Remove_Trailing_Semicolon_283(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Remove_Trailing_Semicolon_283.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/SourceMgr.h"

// ========================================================================================================
#define MUT283_OUTPUT 1

void MutatorFrontendAction_Remove_Trailing_Semicolon_283::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::Stmt>(("Stmt"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = llvm::StringRef::trim(llvm::StringRef(Result.SourceManager->getBufferData(
                     MT->getSourceRange().getBegin(), MT->getSourceRange().getEnd()).data(),
                     MT->getSourceRange().getEnd().getOffset() -
                     MT->getSourceRange().getBegin().getOffset()));
      //Perform mutation on the source code text by applying string replacement
      if (declaration.back() == ';') {
        declaration = declaration.drop_back();
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), declaration);
    }
}
  
void MutatorFrontendAction_Remove_Trailing_Semicolon_283::MutatorASTConsumer_Remove_Trailing_Semicolon_283::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = stmt().bind("Stmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}