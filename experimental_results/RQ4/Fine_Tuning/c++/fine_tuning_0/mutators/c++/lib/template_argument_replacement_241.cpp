//source file
#include "../include/Template_Argument_Replacement_241.h"

// ========================================================================================================
#define MUT241_OUTPUT 1

void MutatorFrontendAction_241::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateArgumentLoc>("TemplateArgs")) {
      //Filter nodes in header files
      if (!MT)
        return;
      //Get the source code text of target node
      auto type = MT->getArgument().getAsType();
      if (!type->isBuiltinType())
        return;
      //Perform mutation on the source code text by applying string replacement
      string replacement = "Incomplete";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), replacement);
    }
}
  
void MutatorFrontendAction_241::MutatorASTConsumer_241::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = templateArgumentLoc().bind("TemplateArgs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}