//source file
#include "../include/insert_unused_variable_225.h"

// ========================================================================================================
#define MUT225_OUTPUT 1

void MutatorFrontendAction_225::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        auto functionBody = FD->getBody();
        auto bodyText = stringutils::rangetoStr(*(Result.SourceManager),
                                                functionBody->getSourceRange());
        std::string unusedVar = "\n/*mut225*/int unused_var_225 = 0;\n";
        if (bodyText.find("{") != std::string::npos) {
          bodyText.insert(bodyText.find("{") + 1, unusedVar);
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(functionBody->getSourceRange()), bodyText);
        }
      }
    }
}
  
void MutatorFrontendAction_225::MutatorASTConsumer_225::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}