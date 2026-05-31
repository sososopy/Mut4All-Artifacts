//header file
#pragma once
#include "Mutator_base.h"

/**
 * Negate_Conditional_Statement_189
 */ 
class MutatorFrontendAction_Negate_Conditional_Statement_189 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Negate_Conditional_Statement_189)

private:
    class MutatorASTConsumer_Negate_Conditional_Statement_189 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Negate_Conditional_Statement_189(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Negate_Conditional_Statement_189.h"

// ========================================================================================================
#define NEGATE_CONDITIONAL_STATEMENT_189_OUTPUT 1

void MutatorFrontendAction_Negate_Conditional_Statement_189::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *IfStmt = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt")) {
      //Filter nodes in header files
      if (!IfStmt || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IfStmt->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto condition = stringutils::rangetoStr(*(Result.SourceManager), IfStmt->getCond()->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto mutatedCondition = "!(" + condition + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(IfStmt->getCond()->getSourceRange()), mutatedCondition);
    } else if (auto *CondExpr = Result.Nodes.getNodeAs<clang::ConditionalOperator>("CondExpr")) {
      //Filter nodes in header files
      if (!CondExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CondExpr->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto condition = stringutils::rangetoStr(*(Result.SourceManager), CondExpr->getCond()->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto mutatedCondition = "!(" + condition + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CondExpr->getCond()->getSourceRange()), mutatedCondition);
    }
}

void MutatorFrontendAction_Negate_Conditional_Statement_189::MutatorASTConsumer_Negate_Conditional_Statement_189::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher ifStmtMatcher = ifStmt().bind("IfStmt");
    StatementMatcher condExprMatcher = conditionalOperator().bind("CondExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(ifStmtMatcher, &callback);
    matchFinder.addMatcher(condExprMatcher, &callback);
    matchFinder.matchAST(Context);
}