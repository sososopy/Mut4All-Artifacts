//source file
#include "../include/modify_friend_function_default_argument_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FriendFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      for (unsigned i = 0; i < FD->getNumParams(); ++i) {
          auto *param = FD->getParamDecl(i);
          if (param->hasDefaultArg()) {
              //Perform mutation on the source code text by applying string replacement
              std::string newDefaultArg = "10"; // Example new default argument
              std::string newParamText = param->getType().getAsString() + " = " + newDefaultArg;
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(param->getSourceRange(), newParamText);
              break; // Modify only one default argument
          }
      }
    }
}
  
void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasParent(cxxRecordDecl(isTemplateInstantiation()))).bind("FriendFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}