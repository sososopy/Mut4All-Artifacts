//source file
#include "../include/replace_while_with_recursive_function_31.h"

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *WS = Result.Nodes.getNodeAs<clang::WhileStmt>("WhileStmt")) {
      if (!WS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     WS->getBeginLoc()))
        return;

      auto condition = stringutils::rangetoStr(*(Result.SourceManager), WS->getCond()->getSourceRange());
      auto body = stringutils::rangetoStr(*(Result.SourceManager), WS->getBody()->getSourceRange());

      functionName = "recursiveFunction_" + std::to_string(rand() % 1000);
      std::string recursiveFunction = "void " + functionName + "() {\n" +
                                      "if (" + condition + ") {\n" +
                                      body + "\n" +
                                      functionName + "();\n" +
                                      "}\n" +
                                      "}\n";

      Rewrite.InsertTextBefore(WS->getBeginLoc(), recursiveFunction);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(WS->getSourceRange()), functionName + "();");
    }
}
  
void MutatorFrontendAction_31::MutatorASTConsumer_31::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = whileStmt().bind("WhileStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}