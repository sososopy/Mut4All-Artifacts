//source file
#include "../include/Add_Attribute_To_Lambda_Expression_483.h"

// ========================================================================================================
#define MUT483_OUTPUT 1

void MutatorFrontendAction_483::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!LT || !Result.Context->getSourceManager().isInMainFile(LT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = Lexer::getSourceText(CharSourceRange::getTokenRange(LT->getSourceRange()), *Result.SourceManager, LangOptions());
      std::string declarationStr(declaration.begin(), declaration.end());
      //Perform mutation on the source code text by applying string replacement
      std::vector<std::string> calling_conventions = {"aapcs", "aapcs-vfp", "intel"};
      std::string attribute = "__attribute__((pcs(\"" + calling_conventions[0] + "\")))";
      size_t pos = declarationStr.find(")");
      if (pos != std::string::npos) {
        declarationStr.insert(pos, " " + attribute);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LT->getSourceRange()), declarationStr);
    }
}
  
void MutatorFrontendAction_483::MutatorASTConsumer_483::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}