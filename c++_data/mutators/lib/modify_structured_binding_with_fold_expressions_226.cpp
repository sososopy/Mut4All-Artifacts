//source file
#include "../include/modify_structured_binding_with_fold_expressions_226.h"

// ========================================================================================================
#define MUT226_OUTPUT 1

void MutatorFrontendAction_226::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BD = Result.Nodes.getNodeAs<clang::DecompositionDecl>("Binding")) {
      //Filter nodes in header files
      if (!BD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BD->getLocation()))
        return;

      // Get the source code text of target node
      auto bindingText = stringutils::rangetoStr(*(Result.SourceManager), BD->getSourceRange());
      
      // Perform mutation on the source code text by applying string replacement
      std::string foldExpr = "(10, ...);"; // Mutated fold expression
      size_t pos = bindingText.find(';');
      if (pos != std::string::npos) {
          bindingText.insert(pos, foldExpr);
      }
      
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BD->getSourceRange()), bindingText);
    }
}

void MutatorFrontendAction_226::MutatorASTConsumer_226::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = decompositionDecl().bind("Binding");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}