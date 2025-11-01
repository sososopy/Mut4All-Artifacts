//source file
#include "../include/Modify_Static_Assertions_In_Anonymous_Structs_238.h"

// ========================================================================================================
#define MUT238_OUTPUT 1

void MutatorFrontendAction_238::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
      //Filter nodes in header files
      if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SA->getLocation()))
        return;

      //Get the source code text of target node
      auto assertionText = stringutils::rangetoStr(*(Result.SourceManager), SA->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string newAssertion = "(__builtin_types_compatible_p(typeof(*mcurrent), double)) || "
                                 "(__builtin_types_compatible_p(typeof(*mcurrent), float))";

      //Replace the original AST node with the mutated one
      size_t startPos = assertionText.find("(__builtin_types_compatible_p");
      if (startPos != std::string::npos) {
          size_t endPos = assertionText.find(")", startPos);
          if (endPos != std::string::npos) {
              assertionText.replace(startPos, endPos - startPos + 1, newAssertion);
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(SA->getSourceRange()), assertionText);
          }
      }
    }
}
  
void MutatorFrontendAction_238::MutatorASTConsumer_238::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = staticAssertDecl().bind("StaticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}