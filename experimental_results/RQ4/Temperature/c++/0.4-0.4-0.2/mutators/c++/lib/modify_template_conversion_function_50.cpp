//source file
#include "../include/modify_template_conversion_function_50.h"

// ========================================================================================================
#define MUT50_OUTPUT 1

void MutatorFrontendAction_50::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("conversionFunc")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isTemplateInstantiation())
        return;

      auto methodBody = MT->getBody();
      if (!methodBody)
        return;

      std::string replacement = "/*mut50*/ operator double();";
      Rewrite.ReplaceText(methodBody->getSourceRange(), replacement);
    }
}
  
void MutatorFrontendAction_50::MutatorASTConsumer_50::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isTemplateInstantiation(), hasName("operator"), hasBody(stmt())).bind("conversionFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}