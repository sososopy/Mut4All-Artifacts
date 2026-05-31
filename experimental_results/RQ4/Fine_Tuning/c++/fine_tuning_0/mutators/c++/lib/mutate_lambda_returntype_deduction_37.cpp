//source file
#include "../include/Mutate_Lambda_ReturnType_Deduction_37.h"

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (content.find("decltype") != string::npos) {
        if (content.find("decltype") != content.rfind("decltype"))
          return;
        if (content.find("return") != content.npos)
          return;
        auto param = DL->getLambdaClass()->getLambdaCallOperator()->getParamDecl(
            0); // assume there is only one parameter
        auto param_name = param->getNameAsString();
        if (content.find(param_name) == content.npos)
          return;
        auto param_type = param->getType();
        if (param_type->isReferenceType() == false)
          return;
        if (param_type->getPointeeType().isConstQualified() == false)
          return;
        string target =
            "decltype(std::remove_const_t<std::remove_reference_t<decltype(" +
            param_name + ")>>::func())";
        int pos = content.find("decltype");
        int len = string("decltype").length();
        content.replace(pos, len, target);
        llvm::outs() << content << '\n';
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(DL->getSourceRange()), content);
      }
    }
}
  
void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}