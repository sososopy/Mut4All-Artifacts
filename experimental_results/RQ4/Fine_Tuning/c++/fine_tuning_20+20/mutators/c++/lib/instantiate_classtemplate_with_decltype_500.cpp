//source file
#include "../include/instantiate_classtemplate_with_decltype_500.h"

// ========================================================================================================
#define MUT500_OUTPUT 1

void MutatorFrontendAction_500::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    cur_templates.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isVirtual())
      return;
    if (!DL->isDefinedOutsideFunctionOrMethod())
      return;
    auto DL_parent = DL->getParent();
    if (!DL_parent->isCompleteDefinition())
      return;
    if (DL_parent->getDescribedClassTemplate() == nullptr)
      return;
    auto fields = DL_parent->fields();
    if (fields.begin() == fields.end())
      return;
    auto target_field = *(fields.begin());
    auto DL_name = DL->getNameAsString();
    // DL->getReturnType().dump();
    auto DL_type = DL->getReturnType().getAsString();
    auto DL_decl = stringutils::rangetoStr(*(Result.SourceManager),
                                           DL->getSourceRange());
    llvm::outs() << DL_decl << '\n';
    DL_decl = "decltype(" + DL_parent->getNameAsString() + "::" +
              target_field->getNameAsString() + ")&";
    DL_decl += DL_name;
    DL_decl += "() {";
    DL_decl += DL_type + " mut_500 = {};";
    DL_decl += "return mut_500;}";
    DL_decl = "/*mut500*/" + DL_decl;
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                        DL_decl);
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>(
                     "Specializations")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (!DL->isCompleteDefinition())
      return;
    auto DL_name = DL->getNameAsString();
    llvm::outs() << DL_name << '\n';
    for (std::size_t j = 0; j < cur_templates.size(); ++j) {
      auto cur_template = cur_templates[j];
      auto cur_name = cur_template->getNameAsString();
      if (cur_name != DL_name)
        continue;
      auto DL_args = DL->getTemplateArgs().asArray();
      if (DL_args.size() != 1)
        return;
      auto DL_arg = DL_args.front();
      auto DL_type = DL_arg.getAsType();
      auto DL_type_str = DL_type.getAsString();
      llvm::outs() << DL_type_str << '\n';
      auto DL_decl = "template class " + DL_name + "<" + DL_type_str + ">;";
      DL_decl = "/*mut500*/" + DL_decl;
      Rewrite.ReplaceText(DL->getEndLoc(), 0, DL_decl);
    }
  }
}
  
void MutatorFrontendAction_500::MutatorASTConsumer_500::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
  auto method_matcher = cxxMethodDecl().bind("Methods");
  auto specialization_matcher =
      classTemplateSpecializationDecl().bind("Specializations");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(method_matcher, &callback);
  matchFinder.addMatcher(specialization_matcher, &callback);
  matchFinder.matchAST(Context);
}