//source file
#include "../include/Mutator_ReplaceMacroArguments_317.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"

// ========================================================================================================
#define MUT317_OUTPUT 1

void MutatorFrontendAction_317::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>({{"MacroInvocation"}})) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace the arguments with different types of initializers
      std::string mutatedDeclaration = declaration;
      size_t openParenPos = declaration.find('(');
      size_t closeParenPos = declaration.find(')');
      if (openParenPos != std::string::npos && closeParenPos != std::string::npos) {
        std::string args = declaration.substr(openParenPos + 1, closeParenPos - openParenPos - 1);
        // Replace the arguments with a braced initializer list
        mutatedDeclaration = declaration.substr(0, openParenPos + 1) + "{" + args + "}" + declaration.substr(closeParenPos);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_317::MutatorASTConsumer_317::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::callExpr().bind("MacroInvocation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}