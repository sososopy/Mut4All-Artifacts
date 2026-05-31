//source file
#include "../include/modify_template_conversion_function_245.h"

// ========================================================================================================
#define MUT245_OUTPUT 1

void MutatorFrontendAction_245::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MethodWithConversion")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->hasBody()) {
        auto body = MT->getBody();
        if (body) {
          std::string replacement = "operator double();";
          Rewrite.ReplaceText(body->getSourceRange(), replacement);
        }
      }
    }
}
  
void MutatorFrontendAction_245::MutatorASTConsumer_245::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(hasDescendant(cxxConversionDecl())).bind("MethodWithConversion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}