//source file
#include "../include/faulty_template_parameter_mutation_302.h"

// ========================================================================================================
#define MUT302_OUTPUT 1

void MutatorFrontendAction_302::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
    if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TP->getLocation()))
      return;
    cur_templates.push_back(TP);
    cur_parameters.push_back(TP->getTemplateParameters());
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    if (DL->isLambda())
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    auto class_name = DL->getNameAsString();
    if (class_name == "")
      return;
    for (std::size_t i = 0; i < cur_templates.size(); ++i) {
      if (cur_templates[i]->getTemplatedDecl() == DL) {
        auto paramlist = cur_parameters[i];
        auto param_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                     paramlist->getSourceRange());
        llvm::outs() << param_content << '\n';
        if (paramlist->size() == 1) {
          auto param = paramlist->getParam(0);
          if (param->isTemplateParameterPack()) {
            param_content = "class " + class_name + "...";
          } else {
            param_content = "class ..." + class_name;
          }
        } else if (paramlist->size() > 1) {
          auto param = paramlist->getParam(0);
          if (param->isTemplateParameterPack()) {
            param_content = "class " + class_name + "...";
          } else {
            param_content = "class ..." + class_name;
          }
          param_content += ",";
          for (std::size_t j = 1; j < paramlist->size(); ++j) {
            auto cur_param = paramlist->getParam(j);
            auto cur_param_content = stringutils::rangetoStr(
                *(Result.SourceManager), cur_param->getSourceRange());
            param_content += cur_param_content;
            if (j != paramlist->size() - 1)
              param_content += ",";
          }
        }
        llvm::outs() << param_content << '\n';
        content.replace(content.find('<'), content.rfind('>') - content.find('<') + 1,
                        "<" + param_content + ">");
      }
    }
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
  }
}
  
void MutatorFrontendAction_302::MutatorASTConsumer_302::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
  auto class_matcher = cxxRecordDecl().bind("Classes");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(class_matcher, &callback);
  matchFinder.matchAST(Context);
}