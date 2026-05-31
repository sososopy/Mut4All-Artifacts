//source file
#include "../include/expand_lambda_this_capture_497.h"

// ========================================================================================================
#define MUT497_OUTPUT 1

void MutatorFrontendAction_497::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (content.find("this") != string::npos) {
      cur_classes.push_back(DL);
    }
  } else if (auto *LL = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
    if (!LL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   LL->getBeginLoc()))
      return;
    if (LL->capture_begin() == LL->capture_end())
      return;
    bool this_capture = false;
    for (auto iter = LL->capture_begin(); iter != LL->capture_end(); ++iter) {
      if (iter->capturesThis()) {
        this_capture = true;
        break;
      }
    }
    if (this_capture == false)
      return;
    auto lambda_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                  LL->getSourceRange());
    if (lambda_content.find("this") == string::npos)
      return;
    if (lambda_content.find("return") == string::npos)
      return;
    for (auto cur_class : cur_classes) {
      if (cur_class->getNameAsString() == "Mut_497") {
        return;
      }
    }
    if (lambda_content.find("std::function") != string::npos) {
      return;
    }
    auto ret_type = LL->getCallOperator()->getReturnType();
    if (ret_type->isIntegerType()) {
      lambda_content = std::regex_replace(
          lambda_content, std::regex("return"),
          "return S{.func=[this](){}/*mut497*/");
      lambda_content = std::regex_replace(lambda_content, std::regex(";"),
                                          "};/*mut497*/");
      lambda_content = std::regex_replace(lambda_content, std::regex("int"),
                                          "S/*mut497*/");
    } else if (ret_type->isRecordType()) {
      auto ret_decl = ret_type->getAsCXXRecordDecl();
      if (!ret_decl->isCompleteDefinition())
        return;
      auto ret_name = ret_decl->getNameAsString();
      llvm::outs() << ret_name << '\n';
      auto fields = ret_decl->fields();
      for (auto field : fields) {
        if (field->getType()->isSpecificBuiltinType(BuiltinType::Int)) {
          lambda_content = std::regex_replace(
              lambda_content, std::regex("return"),
              "return " + ret_name + "{.func=[this](){}/*mut497*/");
          lambda_content = std::regex_replace(lambda_content, std::regex(";"),
                                              "};/*mut497*/");
          break;
        }
      }
    }
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(LL->getSourceRange()),
                        lambda_content);
  }
}

void MutatorFrontendAction_497::MutatorASTConsumer_497::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto lambda_matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.matchAST(Context);
}