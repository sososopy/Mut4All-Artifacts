//source file
#include "../include/misconfigure_template_parameter_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Method")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (const auto *RT = MT->getReturnType()->getAs<clang::TemplateSpecializationType>()) {
          auto templateName = RT->getTemplateName().getAsTemplateDecl()->getNameAsString();
          std::string misconfiguredType = "UndefinedType";
          std::string originalReturnType = RT->getTemplateName().getAsTemplateDecl()->getQualifiedNameAsString();
          std::string newReturnType = templateName + "<" + misconfiguredType + ">";
          std::string methodDecl = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
          size_t pos = methodDecl.find(originalReturnType);
          if (pos != std::string::npos) {
              methodDecl.replace(pos, originalReturnType.length(), newReturnType);
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), methodDecl);
          }
      }
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(returns(templateSpecializationType())).bind("Method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}