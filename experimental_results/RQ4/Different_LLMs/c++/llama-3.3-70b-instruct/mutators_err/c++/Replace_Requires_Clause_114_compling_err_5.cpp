//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Expr.h"

/**
 * Replace_Requires_Clause_114
 */ 
class MutatorFrontendAction_114 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(114)

private:
    class MutatorASTConsumer_114 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_114(Rewriter &R) : TheRewriter(R) {}
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

//source file
#include "../include/Mutator_Replace_Requires_Clause_114.h"
#include "clang/AST/ExprCXX.h"
#include "clang/AST/Stmt.h"

// ========================================================================================================
#define MUT114_OUTPUT 1

void MutatorFrontendAction_114::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::Expr>("CXXRequiresExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = llvm::StringRef(stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange()));
      //Perform mutation on the source code text by applying string replacement
      // Remove the rest parameter pack
      declaration = declaration.substr(0, declaration.find("rest...")) + declaration.substr(declaration.find("rest...") + 6);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_114::MutatorASTConsumer_114::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::expr().bind("CXXRequiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}