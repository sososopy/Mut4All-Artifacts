//source file
#include "../include/remove_typename_keyword_in_template_typedef_275.h"

// ========================================================================================================
#define MUT275_OUTPUT 1

void MutatorFrontendAction_275::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DRE = Result.Nodes.getNodeAs<clang::TypeLoc>("DependentName")) {
      //Filter nodes in header files
      if (!DRE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DRE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto startLoc = DRE->getBeginLoc();
      auto endLoc = DRE->getEndLoc();
      auto range = CharSourceRange::getTokenRange(startLoc, endLoc);
      auto text = Lexer::getSourceText(range, *Result.SourceManager, Result.Context->getLangOpts());

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = text.str();
      size_t pos = mutatedText.find("typename ");
      if (pos != std::string::npos) {
          mutatedText.erase(pos, 9); // Remove 'typename ' keyword
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(range, mutatedText);
    }
}
  
void MutatorFrontendAction_275::MutatorASTConsumer_275::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = typeLoc(loc(qualType(hasDeclaration(namedDecl(hasName("DependentName")))))).bind("DependentName");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}