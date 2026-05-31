//source file
#include "../include/invalid_capture_in_lambda_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto var_name = DL->getNameAsString();
      var_names.push_back(var_name);
    } else if (auto *LE =
                   Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      auto capture_list = LE->capture_size();
      auto capture_string = stringutils::rangetoStr(
          *(Result.SourceManager), LE->getSourceRange());
      llvm::outs() << capture_list << '\n';
      std::vector<string> filtered_var_names;
      for (auto var_name : var_names) {
        if (capture_string.find(var_name) == string::npos)
          filtered_var_names.push_back(var_name);
      }
      if (filtered_var_names.empty())
        return;
      auto random_index = getrandom::getRandomIndex(filtered_var_names.size());
      auto insert_capture = filtered_var_names[random_index];
      auto origin = stringutils::rangetoStr(*(Result.SourceManager),
                                            LE->getSourceRange());
      if (capture_list == 0) {
        origin.insert(origin.find(']'), insert_capture);
      } else {
        origin.insert(origin.find(']'), "," + insert_capture);
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()),
                          origin);
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl().bind("VarDecl");
    auto lambda_matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.matchAST(Context);
}