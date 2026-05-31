//source file
#include "../include/mutator_477.h"

// ========================================================================================================
#define MUT477_OUTPUT 1

void MutatorFrontendAction_477::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::BinaryOperator>("ComplexCond")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lhs = stringutils::rangetoStr(*(Result.SourceManager),
                                         MT->getLHS()->getSourceRange());
      auto rhs = stringutils::rangetoStr(*(Result.SourceManager),
                                         MT->getRHS()->getSourceRange());
      auto op = MT->getOpcode();
      string opstr = "";
      if (op == BinaryOperatorKind::BO_LT)
        opstr = "<";
      else if (op == BinaryOperatorKind::BO_GT)
        opstr = ">";
      else if (op == BinaryOperatorKind::BO_LE)
        opstr = "<=";
      else if (op == BinaryOperatorKind::BO_GE)
        opstr = ">=";
      else if (op == BinaryOperatorKind::BO_EQ)
        opstr = "==";
      else if (op == BinaryOperatorKind::BO_NE)
        opstr = "!=";
      else
        return;
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << lhs << ' ' << opstr << ' ' << rhs << '\n';
      string newlhs = "&__real__ " + lhs;
      string newrhs = "&__real__ " + rhs;
      int choice = getrandom::getRandomIndex(1);
      if (choice == 0)
        newlhs = "&__imag__ " + lhs;
      else
        newrhs = "&__imag__ " + rhs;
      string res = newlhs + ' ' + opstr + ' ' + newrhs;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), "/*mut477*/" + res);
    }
}
  
void MutatorFrontendAction_477::MutatorASTConsumer_477::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator(hasLHS(hasType(hasUnqualifiedDesugaredType(
                                         complexType()))))
                       .bind("ComplexCond");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}