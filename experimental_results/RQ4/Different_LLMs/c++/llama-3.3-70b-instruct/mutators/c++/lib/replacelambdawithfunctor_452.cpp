//source file
#include "../include/ReplaceLambdaWithFunctor_452.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"

// ========================================================================================================
#define MUT452_OUTPUT 1

void MutatorFrontendAction_452::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaText = Lexer::getSourceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      std::string functorText = "struct Functor {\n";
      functorText += "  template <typename... Args>\n";
      functorText += "  auto operator()(Args... args) {\n";
      functorText += "    /*mut452*/" + std::string(lambdaText.substr(lambdaText.find("["), lambdaText.find("]"))) + "\n";
      functorText += "    return " + std::string(lambdaText.substr(lambdaText.find("return ") + 7)) + ";\n";
      functorText += "  }\n";
      functorText += "};\n";
      functorText += "Functor()(" + std::string(lambdaText.substr(lambdaText.find("(") + 1, lambdaText.find(")"))) + ");\n";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), functorText);
    }
}
  
void MutatorFrontendAction_452::MutatorASTConsumer_452::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}