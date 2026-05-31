//source file
#include "../include/alias_template_with_nested_templates_53.h"

// ========================================================================================================
#define MUT53_OUTPUT 1

void MutatorFrontendAction_53::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AT = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasTemplate")) {
      if (!AT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AT->getLocation()))
        return;
      auto aliasDecl = AT->getTemplatedDecl();
      if (!aliasDecl)
        return;
      auto underlyingType = aliasDecl->getUnderlyingType().getAsString();
      auto aliasName = aliasDecl->getNameAsString();
      
      std::string templateParams;
      for (const auto *param : *AT->getTemplateParameters()) {
          if (!templateParams.empty()) {
              templateParams += ", ";
          }
          templateParams += param->getNameAsString();
      }
      templateParams = "<" + templateParams + ">";
      
      std::string wrapperStruct = "template" + templateParams + " struct Wrapper_" + aliasName + " { using type = " + underlyingType + "; };";
      std::string newAlias = "template" + templateParams + " using " + aliasName + " = Wrapper_" + aliasName + "<T>::type;";
      
      Rewrite.ReplaceText(AT->getSourceRange(), "/*mut53*/" + wrapperStruct + "\n" + newAlias);
    }
}
  
void MutatorFrontendAction_53::MutatorASTConsumer_53::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}