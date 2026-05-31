//source file
#include "../include/Replace_Default_Argument_65.h"

// ========================================================================================================
#define MUT65_OUTPUT 1

void MutatorFrontendAction_65::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Call = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!Call || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Call->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 Call->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto funcDecl = Call->getDirectCallee();
      if (funcDecl && funcDecl->getKind() == Decl::Function) {
        auto params = funcDecl->parameters();
        int numArgs = Call->getNumArgs();
        int numParams = params.size();
        if (numArgs < numParams) {
          std::string replacement = declaration;
          for (int i = numArgs; i < numParams; ++i) {
            auto param = params[i];
            if (param->hasDefaultArg()) {
              auto defaultArg = param->getDefaultArg();
              if (defaultArg) {
                std::string defaultArgStr = stringutils::rangetoStr(*(Result.SourceManager),
                                                                   defaultArg->getSourceRange());
                replacement.insert(replacement.find(")"), ", " + defaultArgStr);
              }
            }
          }
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(Call->getSourceRange()), replacement);
        }
      }
    }
}
  
void MutatorFrontendAction_65::MutatorASTConsumer_65::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}