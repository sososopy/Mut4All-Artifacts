//source file
#include "../include/Replace_Decltype_With_This_Operator_Call_13.h"

// ========================================================================================================
#define MUT13_OUTPUT 1

void MutatorFrontendAction_13::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::DecltypeTypeLoc>("decltypeLoc")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto sourceRange = MT->getSourceRange();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      //Check if the decltype contains (*this)(arguments) pattern
      size_t pos = originalText.find("(*this)(");
      if (pos == std::string::npos) return;
      
      //Extract the arguments part
      size_t argStart = pos + 8; // Length of "(*this)("
      size_t parenCount = 1;
      size_t argEnd = argStart;
      while (argEnd < originalText.size() && parenCount > 0) {
        if (originalText[argEnd] == '(') parenCount++;
        else if (originalText[argEnd] == ')') parenCount--;
        argEnd++;
      }
      if (parenCount != 0) return;
      
      std::string arguments = originalText.substr(argStart, argEnd - argStart - 1);
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = originalText;
      mutatedText.replace(pos, 8 + arguments.size() + 1, "this->operator()(" + arguments + ")");
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_13::MutatorASTConsumer_13::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = decltypeTypeLoc().bind("decltypeLoc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}