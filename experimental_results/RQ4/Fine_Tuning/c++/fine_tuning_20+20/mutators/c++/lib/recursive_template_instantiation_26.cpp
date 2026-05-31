//source file
#include "../include/recursive_template_instantiation_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *TP = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
    if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TP->getLocation()))
      return;

    cur_templates.push_back(TP);
  } else if (auto *SP = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>(
                 "Specializations")) {
    if (!SP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   SP->getLocation()))
      return;
    cur_specializations.push_back(SP);
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;

    auto DL_name = DL->getNameAsString();
    if (DL_name != "VFunc")
      return;
    llvm::outs() << "VFunc found\n";
    auto DL_def = stringutils::rangetoStr(*(Result.SourceManager),
                                          DL->getSourceRange());
    llvm::outs() << DL_def << '\n';
    if (DL_def.find("typename F") != string::npos) {
      DL_def = DL_def.replace(DL_def.find("typename F"), 9, "typename F, int");
      DL_def = DL_def.replace(DL_def.find("F"), 1, "F, N");
    }
    DL_def = "/*mut26*/" + DL_def;
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                        DL_def);
  }
}

void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(
    ASTContext &Context) {
  // Matchers
  auto TemplateMatcher = classTemplateDecl().bind("Templates");
  auto SpecializationMatcher =
      classTemplateSpecializationDecl().bind("Specializations");
  auto ClassMatcher = cxxRecordDecl().bind("Classes");

  // Add matchers
  MatchFinder Finder;
  Callback callback(TheRewriter);
  Finder.addMatcher(TemplateMatcher, &callback);
  Finder.addMatcher(SpecializationMatcher, &callback);
  Finder.addMatcher(ClassMatcher, &callback);

  Finder.matchAST(Context);
}