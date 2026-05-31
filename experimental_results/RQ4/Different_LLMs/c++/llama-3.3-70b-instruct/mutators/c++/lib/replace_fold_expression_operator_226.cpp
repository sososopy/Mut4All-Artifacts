//source file
#include "../include/Mutator_Replace_Fold_Expression_Operator_226.h"
#include "clang/AST/Expr.h"

// ========================================================================================================
#define MUT226_OUTPUT 1

void MutatorFrontendAction_226::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FE = Result.Nodes.getNodeAs<clang::UnaryOperator>("FoldExpression")) {
      //Filter nodes in header files
      if (!FE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto op = FE->getOpcode();
      std::string op_str;
      switch (op) {
        case clang::UO_Plus:
          op_str = "+";
          break;
        case clang::UO_Minus:
          op_str = "-";
          break;
        case clang::UO_Not:
          op_str = "!";
          break;
        case clang::UO_LNot:
          op_str = "!";
          break;
        case clang::UO_Deref:
          op_str = "*";
          break;
        case clang::UO_AddrOf:
          op_str = "&";
          break;
        default:
          return;
      }
      //Perform mutation on the source code text by applying string replacement
      std::string replacement_op;
      int choice = getrandom::getRandomIndex(5);
      switch (choice) {
        case 0:
          replacement_op = "!";
          break;
        case 1:
          replacement_op = "~";
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
    auto matcher = unaryOperator().bind("FoldExpression");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}