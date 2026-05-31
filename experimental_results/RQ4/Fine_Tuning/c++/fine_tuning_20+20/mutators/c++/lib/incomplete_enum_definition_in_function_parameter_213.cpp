//source file
#include "../include/incomplete_enum_definition_in_function_parameter_213.h"

// ========================================================================================================
#define MUT213_OUTPUT 1

void MutatorFrontendAction_213::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      cur_classes.push_back(CL);
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getNumParams() == 0)
        return;
      if (FD->getDescribedFunctionTemplate() != nullptr)
        return;
      auto parent = FD->getParent();
      if (std::find(cur_classes.begin(), cur_classes.end(), parent) ==
          cur_classes.end())
        return;
      auto params = FD->parameters();
      std::vector<string> existed_enums;
      for (auto decl : parent->decls()) {
        if (auto ED = dyn_cast<EnumDecl>(decl)) {
          existed_enums.push_back(ED->getNameAsString());
        }
      }
      int dice = getrandom::getRandomIndex(params.size() - 1);
      auto target = params[dice];
      auto param_str = stringutils::rangetoStr(*(Result.SourceManager),
                                               target->getSourceRange());
      string enum_name = "";
      if (getrandom::getRandomIndex(1)) {
        enum_name = "E";
        int suffix = 0;
        while (std::find(existed_enums.begin(), existed_enums.end(),
                         enum_name) != existed_enums.end()) {
          suffix++;
          enum_name = "E" + std::to_string(suffix);
        }
      }
      string enum_decl =
          "/*mut213*/enum " + enum_name + "{"; // incomplete enum definition
      llvm::outs() << enum_decl << '\n';
      param_str.replace(0, target->getType().getAsString().length(),
                        enum_decl);
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(target->getSourceRange()), param_str);
    }
}
  
void MutatorFrontendAction_213::MutatorASTConsumer_213::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto function_matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.matchAST(Context);
}