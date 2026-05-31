//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * Loop_Init_Modification_100
 */ 
class MutatorFrontendAction_100 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(100)

private:
    class MutatorASTConsumer_100 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_100(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Loop_Init_Modification_100.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"

// ========================================================================================================
#define MUT100_OUTPUT 1

void MutatorFrontendAction_100::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ForLoop = Result.Nodes.getNodeAs<clang::ForStmt>("ForLoop")) {
      //Filter nodes in header files
      if (!ForLoop || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ForLoop->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto init = ForLoop->getInit();
      if (auto *Cond = dyn_cast<clang::ConditionalOperator>(init)) {
        // Swap the true and false branches of the condition
        auto trueExpr = Cond->getTrueExpr();
        auto falseExpr = Cond->getFalseExpr();
        std::string newCondStr = "(" + Cond->getCond()->getSourceRange().getBegin().printToString(*Result.SourceManager) + ") ? " + falseExpr->getSourceRange().getBegin().printToString(*Result.SourceManager) + " : " + trueExpr->getSourceRange().getBegin().printToString(*Result.SourceManager);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(init->getSourceRange()), newCondStr);
      } else if (auto *Expr = dyn_cast<clang::Expr>(init)) {
        // Change the assigned value to a different constant or a variable that exists in the scope
        auto varName = "x";
        auto newInit = "/*mut100*/" + varName + " = 0";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(init->getSourceRange()), newInit);
      } else if (auto *Stmt = dyn_cast<clang::Stmt>(init)) {
        // Replace the function call with a different function that exists in the code or with a constant value
        auto newInit = "/*mut100*/0";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(init->getSourceRange()), newInit);
      }
    }
}
  
void MutatorFrontendAction_100::MutatorASTConsumer_100::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = forStmt().bind("ForLoop");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}