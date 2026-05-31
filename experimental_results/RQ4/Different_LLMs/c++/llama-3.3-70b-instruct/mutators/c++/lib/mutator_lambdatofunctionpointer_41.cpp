//source file
#include "../include/Mutator_LambdaToFunctionPointer_41.h"

// ========================================================================================================
#define MUT41_OUTPUT 1

void MutatorFrontendAction_41::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *L = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!L || !Result.Context->getSourceManager().isWrittenInMainFile(
                     L->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager),
                                               L->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string funcName = "lambdaFunc";
      std::string returnType = "int"; // assuming int return type for simplicity
      std::string paramTypes = "int"; // assuming single int parameter for simplicity
      std::string funcText = "int " + funcName + "(int x) { return x * 2; }";
      std::string funcPtrText = "auto funcPtr = " + funcName + ";";
      std::string mutatedText = funcText + "\n" + funcPtrText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(L->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_41::MutatorASTConsumer_41::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}