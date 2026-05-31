//source file
#include "../include/Add_Member_Function_To_Class_Template_And_Invoke_With_Invalid_Argument_45.h"

// ========================================================================================================
#define MUT45_OUTPUT 1

void MutatorFrontendAction_45::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplates")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      if (!CT->isThisDeclarationADefinition())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), CT->getSourceRange());
      llvm::outs() << content << '\n';
      content.insert(content.rfind('}'),
                     "\n/*mut45*/template<typename U> U mut45() { return U(); "
                     "}");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CT->getSourceRange()),
                          content);
      cur_templates.push_back(CT);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "Functions")) { // Find the main function
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getNameAsString() != "main")
        return;
      if (!FD->isThisDeclarationADefinition())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      llvm::outs() << content << '\n';
      for (auto CT : cur_templates) {
        auto name = CT->getNameAsString();
        content.insert(content.rfind('}'),
                       "\n/*mut45*/" + name + "<int> mut45_obj; mut45_obj."
                       "mut45<45>();");
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()),
                          content);
    }
}

void MutatorFrontendAction_45::MutatorASTConsumer_45::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("ClassTemplates");
    auto func_matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}