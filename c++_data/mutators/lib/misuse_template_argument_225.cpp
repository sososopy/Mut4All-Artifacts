//source file
#include "../include/misuse_template_argument_225.h"

// ========================================================================================================
#define MUT225_OUTPUT 1

void MutatorFrontendAction_225::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UA = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
      if (!UA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UA->getLocation()))
        return;

      if (const auto *TT = UA->getUnderlyingType()->getAs<clang::TemplateSpecializationType>()) {
        if (TT->getTemplateName().getAsTemplateDecl()->getNameAsString() == "variant") {
          std::string originalText = UA->getNameAsString();
          auto args = TT->template_arguments();
          if (args.size() > 1) {
            std::string mutatedText;
            for (size_t i = 0; i < args.size() - 1; ++i) {
              if (i > 0) mutatedText += ", ";
              mutatedText += args[i].getAsType().getAsString();
            }
            std::string newAlias = "using " + UA->getNameAsString() + " = std::variant<" + mutatedText + ">;";
            Rewrite.ReplaceText(UA->getSourceRange(), newAlias);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_225::MutatorASTConsumer_225::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasDecl(hasType(templateSpecializationType(hasTemplateArgument(0, refersToType(asString("variant")))))).bind("Alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}