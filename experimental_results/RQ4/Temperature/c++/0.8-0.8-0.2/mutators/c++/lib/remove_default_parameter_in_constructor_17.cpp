//source file
#include "../include/remove_default_parameter_in_constructor_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CtorDecl = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("ConstructorWithDefaultParam")) {
      //Filter nodes in header files
      if (!CtorDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CtorDecl->getLocation()))
        return;

      for (auto Param : CtorDecl->parameters()) {
        if (Param->hasDefaultArg()) {
          ParamToRemove = Param;
          break;
        }
      }

      if (!ParamToRemove)
        return;

      //Get the source code text of target node
      auto ParamRange = CharSourceRange::getTokenRange(ParamToRemove->getDefaultArgRange());
      //Perform mutation on the source code text by applying string replacement
      Rewrite.RemoveText(ParamRange);
      //Replace the original AST node with the mutated one (done by the rewriter's RemoveText)
    }
}
  
void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(
        forEachDescendant(parmVarDecl(hasDefaultArgument()).bind("ParamWithDefault"))
    ).bind("ConstructorWithDefaultParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}