//source file
#include "../include/Replace_Function_Argument_With_Ellipsis_236.h"

// ========================================================================================================
#define MUT236_OUTPUT 1

void MutatorFrontendAction_236::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithArgs")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto left_bracket = declaration.find('(');
      auto right_bracket = declaration.find(')');
      declaration.replace(left_bracket + 1, right_bracket - left_bracket - 1,
                          "...");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          declaration);
    }
}
  
void MutatorFrontendAction_236::MutatorASTConsumer_236::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAnyParameter(hasDescendant(parmVarDecl()))).bind("FunctionWithArgs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}