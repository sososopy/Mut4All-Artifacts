//source file
#include "../include/ReplaceLambdaWithFunctionPointer_71.h"

// ========================================================================================================
#define MUT71_OUTPUT 1

void MutatorFrontendAction_71::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace lambda with function pointer
      std::string funcPtrDecl = "void (*func_ptr)() = nullptr;";
      std::string mutatedCode = declaration;
      size_t pos = mutatedCode.find("auto");
      if (pos != std::string::npos) {
        mutatedCode.replace(pos, 4, funcPtrDecl);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedCode);
    }
}
  
void MutatorFrontendAction_71::MutatorASTConsumer_71::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::StatementMatcher matcher = clang::ast_matchers::lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}