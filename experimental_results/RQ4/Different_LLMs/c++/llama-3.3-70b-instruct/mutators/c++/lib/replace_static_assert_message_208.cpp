//source file
#include "../include/Mutator_Replace_Static_Assert_Message_208.h"

// ========================================================================================================
#define MUT208_OUTPUT 1

void MutatorFrontendAction_208::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
      //Filter nodes in header files
      if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SA->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   SA->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find_first_of('"');
      size_t endPos = declaration.find_last_of('"');
      if (pos != std::string::npos && endPos != std::string::npos) {
        std::string mutatedMessage = "";
        // Introduce a syntax error by starting the message with a keyword
        mutatedMessage = "static_assert";
        declaration.replace(pos + 1, endPos - pos - 1, mutatedMessage);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SA->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_208::MutatorASTConsumer_208::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = staticAssertDecl().bind("StaticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}