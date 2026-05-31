//source file
#include "../include/convert_non_constexpr_to_constexpr_419.h"

// ========================================================================================================
#define MUT419_OUTPUT 1

void MutatorFrontendAction_419::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("NonConstexprFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (!FD->isConstexpr() && FD->hasBody() && FD->getReturnType()->isBuiltinType()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        if (declaration.find("constexpr") == std::string::npos) {
          declaration.insert(declaration.find("int"), "constexpr ");
          nonConstexprFunctions.push_back(FD);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
        }
      }
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      for (auto *FD : nonConstexprFunctions) {
        if (CE->getDirectCallee() == FD) {
          //Get the source code text of target node
          auto callExpr = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
          //Perform mutation on the source code text by applying string replacement
          std::string constexprCall = "constexpr auto result = " + callExpr + ";";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), constexprCall);
        }
      }
    }
}
  
void MutatorFrontendAction_419::MutatorASTConsumer_419::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher functionMatcher = functionDecl(unless(isConstexpr())).bind("NonConstexprFunction");
    StatementMatcher callMatcher = callExpr().bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}