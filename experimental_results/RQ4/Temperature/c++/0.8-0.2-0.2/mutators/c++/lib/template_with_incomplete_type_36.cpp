//source file
#include "../include/template_with_incomplete_type_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      auto templateName = TD->getNameAsString();
      auto location = TD->getEndLoc();

      std::string forwardDecl = "enum class ForwardDeclaredEnum;\n";
      std::string staticAssert = "static_assert(" + templateName + "<ForwardDeclaredEnum>::value, \"Should be a class\");\n";
      
      Rewrite.InsertTextAfterToken(location, "/*mut36*/\n" + forwardDecl + staticAssert);
    }
}
  
void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}