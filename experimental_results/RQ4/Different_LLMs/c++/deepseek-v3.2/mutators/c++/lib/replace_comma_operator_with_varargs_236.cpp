//source file
#include "../include/Mutator_Replace_Comma_Operator_With_Varargs_236.h"

// ========================================================================================================
#define MUT236_OUTPUT 1

void MutatorFrontendAction_236::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("CommaOperator")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isOverloadedOperator() || FD->getOverloadedOperator() != clang::OO_Comma)
        return;
      if (FD->getNumParams() < 2)
        return;
      //Record the node information to be used in the mutation process
      commaOperators.push_back(FD);
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CXXOperatorCallExpr>("CommaUsage")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getExprLoc()))
        return;
      if (CE->getOperator() != clang::OO_Comma)
        return;
      auto *FD = CE->getDirectCallee();
      if (!FD)
        return;
      //Get the source code text of target node
      for (auto *CommaOp : commaOperators) {
        if (CommaOp == FD) {
          auto paramDecls = FD->parameters();
          if (paramDecls.size() < 2)
            return;
          auto secondParam = paramDecls[1];
          auto paramRange = secondParam->getSourceRange();
          auto paramText = stringutils::rangetoStr(*(Result.SourceManager), paramRange);
          //Perform mutation on the source code text by applying string replacement
          size_t typeEnd = paramText.find_last_of(' ');
          if (typeEnd == std::string::npos)
            return;
          std::string mutatedParam = paramText.substr(0, typeEnd) + "...";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(paramRange, mutatedParam);
          return;
        }
      }
    }
}
  
void MutatorFrontendAction_236::MutatorASTConsumer_236::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher operatorMatcher = functionDecl(hasOverloadedOperatorName(",")).bind("CommaOperator");
    StatementMatcher usageMatcher = cxxOperatorCallExpr(hasOperatorName(",")).bind("CommaUsage");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(operatorMatcher, &callback);
    matchFinder.addMatcher(usageMatcher, &callback);
    matchFinder.matchAST(Context);
}