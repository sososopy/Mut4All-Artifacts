//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_compound_statement_in_conditional_expression_25
 */ 
class MutatorFrontendAction_25 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(25)

private:
    class MutatorASTConsumer_25 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_25(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/modify_compound_statement_in_conditional_expression_25.h"

// ========================================================================================================
#define MUT25_OUTPUT 1

void MutatorFrontendAction_25::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CS = Result.Nodes.getNodeAs<clang::StmtExpr>("CompoundStmt")) {
      //Filter nodes in header files
      if (!CS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CS->getLParenLoc()))
        return;
      
      //Get the source code text of target node
      std::string compoundStmtText = Lexer::getSourceText(
          CharSourceRange::getTokenRange(CS->getSourceRange()),
          *Result.SourceManager, Result.Context->getLangOpts());

      //Perform mutation on the source code text by applying string replacement
      size_t pos = compoundStmtText.find_last_of('}');
      if (pos != std::string::npos) {
          compoundStmtText.insert(pos, " if(false) break;");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CS->getSourceRange(), compoundStmtText);
    }
}
  
void MutatorFrontendAction_25::MutatorASTConsumer_25::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = stmtExpr(has(compoundStmt())).bind("CompoundStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}