//source file
#include "../include/remove_typename_in_template_581.h"

// ========================================================================================================
#define MUT581_OUTPUT 1

void MutatorFrontendAction_581::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeLoc>("DependentType")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      
      //Get the source code text of target node
      auto startLoc = MT->getBeginLoc();
      auto endLoc = MT->getEndLoc();
      auto range = CharSourceRange::getTokenRange(startLoc, endLoc);
      std::string typeText = Lexer::getSourceText(range, *Result.SourceManager, Result.Context->getLangOpts()).str();

      //Perform mutation on the source code text by applying string replacement
      size_t pos = typeText.find("typename ");
      if (pos != std::string::npos) {
          typeText.erase(pos, 9); // Remove "typename "
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(range, typeText);
    }
}
  
void MutatorFrontendAction_581::MutatorASTConsumer_581::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeLocMatcher matcher = typeLoc(loc(qualType(hasDeclaration(namedDecl(hasName("Alias")))))).bind("DependentType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}