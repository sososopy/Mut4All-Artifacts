//source file
#include "../include/add_variadic_template_function_with_lambda_using_decltype_440.h"

// ========================================================================================================
#define MUT440_OUTPUT 1

void MutatorFrontendAction_440::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("Templates")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      cur_templates.push_back(FT);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>(
                   "TranslationUnit")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (llvm::isa<FunctionTemplateDecl>(decl)) {
          return;
        }
      }
      llvm::outs() << "mut440: no template function found\n";
      Rewrite.ReplaceText(DL->getBeginLoc(), 0,
                          "/*mut440*/template <typename F, typename... Ts> void "
                          "mutation_function1(F f) {((Ts{}, f(0)), ...);}\n");
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isTemplateInstantiation())
        return;
      if (FD->hasBody() == false)
        return;
      auto body = FD->getBody();
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             body->getSourceRange());
      auto lambda = "\n/*mut440*/auto lambda=[&](auto)->decltype(f){return 0;};\n";
      if (content.find('{') != string::npos)
        content.insert(content.find('{') + 1, lambda);
      if (content.rfind('}') != string::npos)
        content.insert(content.rfind('}'), "\nlambda(0);\n");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()),
                          content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>(
                   "TranslationUnit")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (llvm::isa<FunctionTemplateDecl>(decl)) {
          return;
        }
      }
      llvm::outs() << "mut440: no template function found\n";
      Rewrite.ReplaceText(DL->getBeginLoc(), 0,
                          "/*mut440*/template <typename F, typename... Ts> void "
                          "mutation_function1(F f) {((Ts{}, f(0)), ...);}\n");
    }
  }

void MutatorFrontendAction_440::MutatorASTConsumer_440::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("Templates");
    auto translationunit_matcher = translationUnitDecl().bind("TranslationUnit");
    auto function_matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(translationunit_matcher, &callback);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.matchAST(Context);
}