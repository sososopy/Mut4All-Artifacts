//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Fold_Expression_Operator_226
 */ 
class MutatorFrontendAction_226 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(226)

private:
    class MutatorASTConsumer_226 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_226(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Fold_Expression_Operator_226.h"

// ========================================================================================================
#define MUT226_OUTPUT 1

void MutatorFrontendAction_226::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FE = Result.Nodes.getNodeAs<clang::FoldExpr>(("FoldExpression"))) {
      //Filter nodes in header files
      if (!FE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FE->getLocation()))
        return;
      //Get the source code text of target node
      auto op = FE->getOperator();
      std::string op_str;
      switch (op) {
        case clang::BO_Comma:
          op_str = ",";
          break;
        case clang::BO_And:
          op_str = "&&";
          break;
        case clang::BO_Or:
          op_str = "||";
          break;
        case clang::BO_Add:
          op_str = "+";
          break;
        case clang::BO_Sub:
          op_str = "-";
          break;
        case clang::BO_Mul:
          op_str = "*";
          break;
        case clang::BO_Div:
          op_str = "/";
          break;
        default:
          return;
      }
      //Perform mutation on the source code text by applying string replacement
      std::string replacement_op;
      int choice = getrandom::getRandomIndex(5);
      switch (choice) {
        case 0:
          replacement_op = "&&";
          break;
        case 1:
          replacement_op = "||";
          break;
        case 2:
          replacement_op = "+";
          break;
        case 3:
          replacement_op = "-";
          break;
        case 4:
          replacement_op = "*";
          break;
      }
      if (op_str == replacement_op)
        return;
      auto mutated_code = std::string(FE->getSourceRange().getBegin().printToString(*Result.SourceManager)) +
                          " (" + replacement_op + " ...) ";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FE->getSourceRange()), mutated_code);
    }
}
  
void MutatorFrontendAction_226::MutatorASTConsumer_226::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    ExprMatcher matcher = foldExpr().bind("FoldExpression");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}