//source file
#include "../include/mutator_303.h"

// ========================================================================================================
#define MUT303_OUTPUT 1

void MutatorFrontendAction_303::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("Specializations")) {
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), TS->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("export") != string::npos)
        return;
      content = "export " + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TS->getSourceRange()), content);
    } else if (auto *TL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                   "Templates")) {
      if (!TL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TL->getLocation()))
        return;
      cur_templates.push_back(TL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("export") != string::npos)
        return;
      if (content.find("template<>") == string::npos)
        return;
      content = "export " + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "EmptyClasses")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("export") != string::npos)
        return;
      if (content.find("template<>") == string::npos)
        return;
      content = "export " + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}

void MutatorFrontendAction_303::MutatorASTConsumer_303::HandleTranslationUnit(
    ASTContext &Context) {
  MatchFinder Finder;
  Callback callback(TheRewriter);
  Finder.addMatcher(
      classTemplateSpecializationDecl().bind("Specializations"), &callback);
  Finder.addMatcher(classTemplateDecl().bind("Templates"), &callback);
  Finder.addMatcher(cxxRecordDecl().bind("Classes"), &callback);
  Finder.addMatcher(cxxRecordDecl(isDefinition()).bind("EmptyClasses"),
                    &callback);
  Finder.matchAST(Context);
}