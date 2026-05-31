//source file
#include "../include/remove_lambda_parameters_and_use_structured_bindings_79.h"

// ========================================================================================================
#define MUT79_OUTPUT 1

void MutatorFrontendAction_79::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      lambdas.push_back(MT);
    } else if (auto *BD = Result.Nodes.getNodeAs<clang::DeclRefExpr>("Bindings")) {
      if (!BD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BD->getBeginLoc()))
        return;
      if (lambdas.empty())
        return;
      auto target_lambda = lambdas.back();
      auto target = target_lambda->getLambdaClass();
      auto lambda_name = target->getNameAsString();
      auto lambda_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                 target_lambda->getSourceRange());
      auto capture_list = target_lambda->capture_size();
      auto params = target_lambda->getLambdaClass()->getLambdaStaticInvoker();
      auto param_list = params->parameters();
      auto body = target_lambda->getBody();
      auto body_text = stringutils::rangetoStr(*(Result.SourceManager),
                                               body->getSourceRange());
      llvm::outs() << lambda_name << '\n';
      llvm::outs() << lambda_text << '\n';
      llvm::outs() << capture_list << '\n';
      llvm::outs() << param_list.size() << '\n';
      llvm::outs() << body_text << '\n';
      if (param_list.size() == 0)
        return;
      auto bd_name = BD->getNameInfo().getAsString();
      llvm::outs() << bd_name << '\n';
      int param_index = 0;
      for (int i = 0; i < param_list.size(); ++i) {
        if (param_list[i]->getNameAsString() == bd_name) {
          param_index = i;
          break;
        }
      }
      llvm::outs() << param_index << '\n';
      if (param_index >= param_list.size())
        return;
      if (param_list.size() == 1) {
        lambda_text.replace(lambda_text.find('('), lambda_text.find(')') - lambda_text.find('(') + 1, "()");
        llvm::outs() << lambda_text << '\n';
      } else if (param_index == 0) {
        lambda_text.replace(lambda_text.find(param_list[param_index]->getNameAsString()), param_list[param_index]->getNameAsString().size() + 1, "");
        llvm::outs() << lambda_text << '\n';
      } else {
        lambda_text.replace(lambda_text.find(param_list[param_index]->getNameAsString()) - 1, param_list[param_index]->getNameAsString().size() + 1, "");
        llvm::outs() << lambda_text << '\n';
      }
      auto bd_text = "auto [" + bd_name + "] = " + lambda_name + "_param;";
      llvm::outs() << bd_text << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BD->getSourceRange()), bd_text);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(target_lambda->getSourceRange()), lambda_text);
    }
}
  
void MutatorFrontendAction_79::MutatorASTConsumer_79::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambdas");
    auto binding_matcher = declRefExpr(to(varDecl(hasType(autoType())))).bind("Bindings");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(binding_matcher, &callback);
    matchFinder.matchAST(Context);
}