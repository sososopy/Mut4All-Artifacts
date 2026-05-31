//source file
#include "../include/Add_Template_Specialization_439.h"

// ========================================================================================================
#define MUT439_OUTPUT 1

void MutatorFrontendAction_439::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;

      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto spec = DL->specializations();
      if (!spec.empty())
        return;
      auto params = DL->getTemplateParameters();
      std::vector<string> param_names;
      for (auto param : *params) {
        if (auto type_param = dyn_cast<TemplateTypeParmDecl>(param)) {
          param_names.push_back(type_param->getNameAsString());
        } else if (auto non_type_param = dyn_cast<NonTypeTemplateParmDecl>(param)) {
          param_names.push_back(non_type_param->getNameAsString());
        } else if (auto template_template_param =
                       dyn_cast<TemplateTemplateParmDecl>(param)) {
          param_names.push_back(template_template_param->getNameAsString());
        }
      }
      if (param_names.empty())
        return;

      string specialization = "template<>";
      if (DL->isThisDeclarationADefinition()) {
        specialization += " struct ";
      } else if (DL->isThisDeclarationADefinition()) {
        specialization += " union ";
      } else {
        assert(false && "Mut439: Unkown template type!");
      }
      specialization += DL->getNameAsString() + "<";
      for (size_t i = 0; i < param_names.size(); ++i) {
        if (i)
          specialization += ",";
        specialization += param_names[i];
      }
      specialization += "> {};\n";
      llvm::outs() << specialization;
      content += "\n/*mut439*/" + specialization;
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                   "FunctionTemplate")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;

      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto spec = DL->specializations();
      if (!spec.empty())
        return;
      auto params = DL->getTemplateParameters();
      std::vector<string> param_names;
      for (auto param : *params) {
        if (auto type_param = dyn_cast<TemplateTypeParmDecl>(param)) {
          param_names.push_back(type_param->getNameAsString());
        } else if (auto non_type_param = dyn_cast<NonTypeTemplateParmDecl>(param)) {
          param_names.push_back(non_type_param->getNameAsString());
        } else if (auto template_template_param =
                       dyn_cast<TemplateTemplateParmDecl>(param)) {
          param_names.push_back(template_template_param->getNameAsString());
        }
      }
      if (param_names.empty())
        return;

      string specialization = "template<>";
      specialization += DL->getTemplatedDecl()->getReturnType().getAsString() + " ";
      specialization += DL->getNameAsString() + "<";
      for (size_t i = 0; i < param_names.size(); ++i) {
        if (i)
          specialization += ",";
        specialization += param_names[i];
      }
      specialization += ">(";
      auto func = DL->getTemplatedDecl();
      auto func_params = func->parameters();
      for (size_t i = 0; i < func_params.size(); ++i) {
        if (i)
          specialization += ",";
        specialization += func_params[i]->getType().getAsString();
      }
      specialization += "){}";
      llvm::outs() << specialization;
      content += "\n/*mut439*/" + specialization;
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_439::MutatorASTConsumer_439::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto class_template_matcher = classTemplateDecl().bind("ClassTemplate");
    auto function_template_matcher =
        functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(class_template_matcher, &callback);
    matchFinder.addMatcher(function_template_matcher, &callback);
    matchFinder.matchAST(Context);
}