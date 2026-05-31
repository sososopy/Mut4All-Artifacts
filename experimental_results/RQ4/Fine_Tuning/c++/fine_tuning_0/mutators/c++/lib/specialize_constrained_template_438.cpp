//source file
#include "../include/Specialize_Constrained_Template_438.h"

// ========================================================================================================
#define MUT438_OUTPUT 1

void MutatorFrontendAction_438::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if (TD->isThisDeclarationADefinition() == false)
        return;
      if (TD->getTemplatedDecl() == nullptr)
        return;
      if (TD->isOutOfLine() == true)
        return;

      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             TD->getSourceRange());
      auto specs = TD->specializations();
      if (specs.empty())
        return;
      for (auto spec : specs) {
        if (spec->isCompleteDefinition() == false)
          return;
      }
      auto tpl_params = TD->getTemplateParameters();
      if (tpl_params->size() != 1)
        return;
      auto tpl_param = tpl_params->getParam(0);
      if (tpl_param->getKind() != clang::Decl::Kind::TemplateTypeParm)
        return;
      auto tpl_typ_param = llvm::dyn_cast<clang::TemplateTypeParmDecl>(tpl_param);
      auto tpl_typ_param_name = tpl_typ_param->getNameAsString();
      llvm::outs() << tpl_typ_param_name << '\n';
      if (content.find("requires") == string::npos)
        return;
      auto req_pos = content.find("requires");
      auto req_end_pos = content.find('{');
      if (req_end_pos == string::npos)
        return;
      auto req_clause = content.substr(req_pos, req_end_pos - req_pos);
      llvm::outs() << req_clause << '\n';
      string new_req_clause = req_clause + " && Constraint2<" + tpl_typ_param_name + ">";
      llvm::outs() << new_req_clause << '\n';
      content.insert(req_pos, "/*mut438*/template <class " + tpl_typ_param_name +
                                  "> " + new_req_clause + " ");
      content.insert(content.rfind('}'), "\n");
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_438::MutatorASTConsumer_438::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}