//source file
#include "../include/Replace_Lambda_Expression_With_Function_Definition_174.h"

// ========================================================================================================
#define MUT174_OUTPUT 1

void MutatorFrontendAction_174::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      auto lambda = stringutils::rangetoStr(*(Result.SourceManager),
                                            MT->getSourceRange());
      llvm::outs() << lambda << '\n';
      auto lambda_type = MT->getLambdaClass()->getLambdaTypeInfo();
      llvm::outs() << lambda_type->getType().getTypePtr()->getTypeClassName() << '\n';
      auto lambda_callop = MT->getCallOperator();
      auto lambda_callop_returntype =
          lambda_callop->getReturnType().getAsString();
      auto lambda_callop_params = lambda_callop->parameters();
      string lambda_callop_params_str = "";
      for (auto param : lambda_callop_params) {
        lambda_callop_params_str += param->getOriginalType().getAsString() +
                                    " " + param->getNameAsString() + ",";
      }
      if (!lambda_callop_params_str.empty())
        lambda_callop_params_str.pop_back();
      llvm::outs() << lambda_callop_returntype << '\n';
      llvm::outs() << lambda_callop_params_str << '\n';
      auto lambda_capture = MT->capture_begin();
      string lambda_capture_str = "";
      for (; lambda_capture != MT->capture_end(); lambda_capture++) {
        lambda_capture_str +=
            lambda_capture->getCapturedVar()->getType().getAsString() + " " +
            lambda_capture->getCapturedVar()->getNameAsString() + ",";
      }
      if (!lambda_capture_str.empty())
        lambda_capture_str.pop_back();
      llvm::outs() << lambda_capture_str << '\n';
      auto lambda_body = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getBody()->getSourceRange());
      llvm::outs() << lambda_body << '\n';
      string lambda_def = "/*mut174*/";
      if (!lambda_capture_str.empty())
        lambda_def +=
            "[" + lambda_capture_str + "](auto " + lambda_callop_params_str +
            ") -> " + lambda_callop_returntype + " " + lambda_body;
      else
        lambda_def += "[](" + lambda_callop_params_str + ") -> " +
                      lambda_callop_returntype + " " + lambda_body;
      llvm::outs() << lambda_def << '\n';
      Rewrite.ReplaceText(MT->getSourceRange(), lambda_def);
    }
}
  
void MutatorFrontendAction_174::MutatorASTConsumer_174::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}