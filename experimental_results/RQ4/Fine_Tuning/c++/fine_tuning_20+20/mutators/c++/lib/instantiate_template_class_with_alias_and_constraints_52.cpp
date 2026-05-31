//source file
#include "../include/instantiate_template_class_with_alias_and_constraints_52.h"

// ========================================================================================================
#define MUT52_OUTPUT 1

void MutatorFrontendAction_52::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    if (FD->hasBody() == false)
      return;
    auto body = FD->getBody();
    if (cur_templates.size() == 0)
      return;
    size_t index = getrandom::getRandomIndex(cur_templates.size() - 1);
    auto target = cur_templates[index];
    auto target_name = target->getNameAsString();
    string ins = "/*mut52*/";
    ins += target_name;
    ins += " mut_52{0};\n";
    Rewrite.ReplaceText(body->getBeginLoc(), 0, ins);
  } else if (auto *TD =
                 Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
    if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TD->getLocation()))
      return;
    if (TD->getTemplatedDecl()->getKind() != Decl::Kind::ClassTemplate)
      return;
    cur_templates.push_back(TD);
  }
}
  
void MutatorFrontendAction_52::MutatorASTConsumer_52::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto function_matcher = functionDecl().bind("Functions");
    auto template_matcher = clang::ast_matchers::decl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.matchAST(Context);
}