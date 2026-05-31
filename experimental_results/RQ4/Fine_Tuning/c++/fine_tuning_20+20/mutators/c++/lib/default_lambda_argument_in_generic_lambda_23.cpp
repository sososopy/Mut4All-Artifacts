//source file
#include "../include/default_lambda_argument_in_generic_lambda_23.h"

// ========================================================================================================
#define MUT23_OUTPUT 1

void MutatorFrontendAction_23::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      if (LE->isGenericLambda() == false)
        return;
      auto params = LE->getLambdaClass()->getLambdaCallOperator()->parameters();
      for (auto param : params) {
        if (param->hasDefaultArg()) {
          auto defaultarg =
              stringutils::rangetoStr(*(Result.SourceManager),
                                      param->getDefaultArgRange());
          cur_params.push_back(defaultarg);
        } else {
          cur_params.push_back("");
        }
      }
      cur_lambdas.push_back(LE);
    } else if (auto *PL = Result.Nodes.getNodeAs<clang::ParmVarDecl>(
                   "Parameters")) {
      if (!PL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PL->getBeginLoc()))
        return;
      int index = -1;
      for (int i = 0; i < cur_lambdas.size(); ++i) {
        if (PL->getParentFunctionOrMethod() ==
            cur_lambdas[i]->getLambdaClass()->getLambdaCallOperator()) {
          index = i;
          break;
        }
      }
      if (index == -1)
        return;
      auto default_value = cur_params[index];
      llvm::outs() << default_value << '\n';
      if (default_value == "") {
        if (PL->getType()->isIntegralType(*Result.Context)) {
          default_value = "0";
        } else if (PL->getType()->isFloatingType()) {
          default_value = "0.0";
        } else if (PL->getType()->isAnyCharacterType()) {
          default_value = "'\\0'";
        } else if (PL->getType()->isPointerType()) {
          default_value = "nullptr";
        } else if (PL->getType()->isRecordType()) {
          default_value = "{}";
        } else
          default_value = "";
      }
      if (default_value != "") {
        Rewrite.ReplaceText(PL->getEndLoc(), 0, "/*mut23*/=" + default_value);
      }
    }
}
  
void MutatorFrontendAction_23::MutatorASTConsumer_23::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdamatcher = lambdaExpr().bind("Lambdas");
    auto param_matcher = parmVarDecl().bind("Parameters");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdamatcher, &callback);
    matchFinder.addMatcher(param_matcher, &callback);
    matchFinder.matchAST(Context);
}