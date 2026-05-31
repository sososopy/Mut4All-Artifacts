//source file
#include "../include/Mutator_InvalidUnaryOpInsertion_203.h"

// ========================================================================================================
#define INVALIDUNARYOPINSERTION203_OUTPUT 1

void MutatorFrontendAction_InvalidUnaryOpInsertion_203::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UE = Result.Nodes.getNodeAs<clang::UnaryOperator>(("UnaryExpr"))) {
      //Filter nodes in header files
      if (!UE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UE->getOperatorLoc()))
        return;
      //Get the source code text of target node
      auto op = UE->getOpcode();
      string op_str = UE->getOpcodeStr();
      //Perform mutation on the source code text by applying string replacement
      string mutated_op_str = "*"; // Insert an invalid unary operator
      string mutated_expr = mutated_op_str + " " + op_str;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(UE->getSourceRange()), mutated_expr);
    }
}
  
void MutatorFrontendAction_InvalidUnaryOpInsertion_203::MutatorASTConsumer_InvalidUnaryOpInsertion_203::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = unaryOperator().bind("UnaryExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}