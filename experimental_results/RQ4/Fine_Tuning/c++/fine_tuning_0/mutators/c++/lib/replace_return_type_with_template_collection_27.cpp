//source file
#include "../include/Replace_Return_Type_With_Template_Collection_27.h"

// ========================================================================================================
#define MUT27_OUTPUT 1

void MutatorFrontendAction_27::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto returnType = MT->getReturnType();
      if (returnType->isVoidType())
        return;
      auto returnTypeStr = stringutils::rangetoStr(
          *(Result.SourceManager), MT->getReturnTypeSourceRange());
      llvm::outs() << returnTypeStr << '\n';
      auto funcName = MT->getNameAsString();
      auto funcBody = stringutils::rangetoStr(
          *(Result.SourceManager), MT->getBody()->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      string new_return_type = "MyVector<" + returnTypeStr + ">";
      string new_func_body = "{ return {}; }";
      auto funcDecl = "/*mut27*/" + new_return_type + " " + funcName +
                      "() " + new_func_body;
      llvm::outs() << funcDecl << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), funcDecl);
    }
}
  
void MutatorFrontendAction_27::MutatorASTConsumer_27::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}