//source file
#include "../include/Expand_Pack_In_Lambda_Within_Consteval_Function_314.h"

// ========================================================================================================
#define MUT314_OUTPUT 1

void MutatorFrontendAction_314::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      llvm::outs() << "Find lambda\n";
      auto lambda = stringutils::rangetoStr(*(Result.SourceManager),
                                            LE->getSourceRange());
      auto captures = LE->captures();
      auto params = LE->getLambdaClass()->getLambdaCallOperator()->parameters();
      if (captures.begin() == captures.end())
        return;
      if (params.size() == 0)
        return;
      auto first_capture = *captures.begin();
      auto first_param = params[0];
      auto first_capture_name = first_capture.getCapturedVar()->getNameAsString();
      auto first_param_name = first_param->getNameAsString();
      if (first_capture_name != first_param_name)
        return;
      auto first_capture_type = first_capture.getCapturedVar()->getType();
      if (!first_capture_type->isDependentType())
        return;
      llvm::outs() << first_capture_name << '\n';
      auto first_capture_type_str = stringutils::rangetoStr(
          *(Result.SourceManager), first_capture.getCapturedVar()->getTypeSourceInfo()->getTypeLoc().getSourceRange());
      llvm::outs() << first_capture_type_str << '\n';
      auto first_param_type_str = stringutils::rangetoStr(
          *(Result.SourceManager), first_param->getTypeSourceInfo()->getTypeLoc().getSourceRange());
      llvm::outs() << first_param_type_str << '\n';
      if (first_capture_type_str != first_param_type_str)
        return;
      auto capture_type_str = first_capture_type_str;
      auto capture_type_str_prefix = capture_type_str.substr(
          0, capture_type_str.find("<") + 1); // prefix<Is>
      auto capture_type_str_suffix = capture_type_str.substr(
          capture_type_str.rfind(">")); // >
      auto capture_type_str_middle = capture_type_str.substr(
          capture_type_str.find("<") + 1,
          capture_type_str.rfind(">") -
              capture_type_str.find("<") -
              1); // Is
      llvm::outs() << capture_type_str_prefix << '\n';
      llvm::outs() << capture_type_str_middle << '\n';
      llvm::outs() << capture_type_str_suffix << '\n';
      if (capture_type_str_middle.find("...") == string::npos)
        return;
      auto capture_type_str_middle_prefix = capture_type_str_middle.substr(
          0, capture_type_str_middle.find("...")); // Is
      llvm::outs() << capture_type_str_middle_prefix << '\n';
      auto capture_type_str_middle_suffix = capture_type_str_middle.substr(
          capture_type_str_middle.find("...") + 3); // 
      llvm::outs() << capture_type_str_middle_suffix << '\n';
      auto capture_type_str_middle_expansion =
          capture_type_str_middle_prefix + capture_type_str_middle_suffix;
      llvm::outs() << capture_type_str_middle_expansion << '\n';
      auto param_type_expansion = capture_type_str_prefix +
                                  capture_type_str_middle_expansion +
                                  capture_type_str_suffix;
      llvm::outs() << param_type_expansion << '\n';
      auto param_expansion = param_type_expansion + " " +
                             first_param_name +
                             capture_type_str_middle_suffix;
      llvm::outs() << param_expansion << '\n';
      auto param_expansion_expansion = param_type_expansion + " " +
                                       first_param_name + "...";
      llvm::outs() << param_expansion_expansion << '\n';
      auto lambda_expansion = lambda;
      lambda_expansion.insert(lambda_expansion.find("](") + 2,
                              param_expansion_expansion + ", ");
      lambda_expansion.insert(lambda_expansion.rfind("}"),
                              "(" + capture_type_str_middle_expansion +
                                  ", 0)");
      llvm::outs() << lambda_expansion << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()),
                          lambda_expansion);
    }
}
  
void MutatorFrontendAction_314::MutatorASTConsumer_314::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isConsteval()).bind("Consteval");
    auto lambda_matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.matchAST(Context);
}