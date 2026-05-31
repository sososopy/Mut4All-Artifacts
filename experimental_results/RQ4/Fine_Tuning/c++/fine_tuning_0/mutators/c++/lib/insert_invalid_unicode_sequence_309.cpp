//source file
#include "../include/Insert_Invalid_Unicode_Sequence_309.h"

// ========================================================================================================
#define MUT309_OUTPUT 1

void MutatorFrontendAction_309::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::StringLiteral>("Strings")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto pos = text.find("\\u");
      if (pos == string::npos) {
        pos = getrandom::getRandomIndex(text.size() - 1);
        text.insert(pos, "\\u");
        pos += 2;
      }
      text.insert(pos + 2, "{");
      text.insert(pos + 5, "}");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), text);
    } else if (auto *MT =
                   Result.Nodes.getNodeAs<clang::CharacterLiteral>("Chars")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      auto pos = text.find("\\u");
      if (pos == string::npos) {
        pos = getrandom::getRandomIndex(text.size() - 1);
        text.insert(pos, "\\u");
        pos += 2;
      }
      text.insert(pos + 2, "{");
      text.insert(pos + 5, "}");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), text);
    }
}
  
void MutatorFrontendAction_309::MutatorASTConsumer_309::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher =
        stringLiteral(hasAncestor(decl())).bind("Strings"); // Find all strings
    auto matcher2 = characterLiteral(hasAncestor(decl())).bind(
        "Chars"); // Find all character constants
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}