//source file
#include "../include/Add_Useless_Variadic_Template_Structure_183.h"

// ========================================================================================================
#define MUT183_OUTPUT 1

void MutatorFrontendAction_183::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (!DL->isTemplateInstantiation())
      return;
    if (DL->isFunctionTemplateSpecialization() == false)
      return;
    if (DL->hasBody() == false)
      return;
    llvm::outs() << DL->getNameAsString() << '\n';
    cur_functions.push_back(DL);
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::FunctionDecl>("Templates")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isTemplateInstantiation())
      return;
    if (DL->isFunctionTemplateSpecialization() == false)
      return;
    if (DL->hasBody() == false)
      return;
    llvm::outs() << DL->getNameAsString() << '\n';
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    content = "/*mut183*/" + content;
    content.insert(content.rfind('}'), "\n[]<typename... Ts>(Ts...){}(0);\n");
    Rewrite.ReplaceText(
        CharSourceRange::getTokenRange(DL->getSourceRange()), content);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                 "VariadicTemplates")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isTemplateInstantiation())
      return;
    if (DL->isFunctionTemplateSpecialization() == false)
      return;
    if (DL->hasBody() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (content.find("typename...") == string::npos)
      return;
    content = "/*mut183*/" + content;
    content.insert(content.rfind('}'), "\n((... , f(Ts{})), 0);\n");
    Rewrite.ReplaceText(
        CharSourceRange::getTokenRange(DL->getSourceRange()), content);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                 "VariadicFunctions")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isTemplateInstantiation())
      return;
    if (DL->isFunctionTemplateSpecialization() == false)
      return;
    if (DL->hasBody() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (content.find("typename...") == string::npos)
      return;
    content = "/*mut183*/" + content;
    content.insert(content.rfind('}'), "\n((... , f(Ts{})), 0);\n");
    Rewrite.ReplaceText(
        CharSourceRange::getTokenRange(DL->getSourceRange()), content);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>(
                 "TranslationUnit")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getBeginLoc()))
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (content.find("typename...") != string::npos)
      return;
    if (cur_functions.empty())
      return;
    auto target = cur_functions[getrandom::getRandomIndex(cur_functions.size() -
                                                          1)];
    string ins = "/*mut183*/template<typename... Ts> void f(Ts...){return;}\n";
    llvm::outs() << target->getNameAsString() << '\n';
    Rewrite.ReplaceText(target->getBeginLoc(), 0, ins);
  }
}
  
void MutatorFrontendAction_183::MutatorASTConsumer_183::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
  auto template_matcher = functionDecl().bind("Templates");
  auto variadic_template_matcher = functionDecl().bind("VariadicTemplates");
  auto variadic_function_matcher = functionDecl().bind("VariadicFunctions");
  auto TU_matcher = translationUnitDecl().bind("TranslationUnit");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(template_matcher, &callback);
  matchFinder.addMatcher(variadic_template_matcher, &callback);
  matchFinder.addMatcher(variadic_function_matcher, &callback);
  matchFinder.addMatcher(TU_matcher, &callback);
  matchFinder.matchAST(Context);
}