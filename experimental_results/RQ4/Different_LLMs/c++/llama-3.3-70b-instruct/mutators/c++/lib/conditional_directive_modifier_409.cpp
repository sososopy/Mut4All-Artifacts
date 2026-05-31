//source file
#include "../include/Conditional_Directive_Modifier_409.h"

// ========================================================================================================
#define MUT409_OUTPUT 1

void MutatorFrontendAction_409::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::IfStmt>(("IfStmt"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto directive = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()),
          *Result.SourceManager, Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      int choice = rand() % 3;
      std::string directiveStr = directive.str();
      if (choice == 0) {
        // Invert the condition
        directiveStr = directiveStr.substr(0, directiveStr.find("if")) + "if (!" + directiveStr.substr(directiveStr.find("if") + 2);
      } else if (choice == 1) {
        // Swap if and else
        size_t ifPos = directiveStr.find("if");
        size_t elsePos = directiveStr.find("else");
        if (elsePos != std::string::npos) {
          directiveStr.replace(ifPos, 2, "el");
          directiveStr.replace(elsePos, 4, "if");
        } else {
          directiveStr.append("\nelse\n#endif");
        }
      } else {
        // Remove or add an else clause
        if (directiveStr.find("else") != std::string::npos) {
          directiveStr.erase(directiveStr.find("else"), 4);
        } else {
          directiveStr.append("\nelse\n#endif");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), directiveStr);
    }
}

void MutatorFrontendAction_409::MutatorASTConsumer_409::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = ifStmt().bind("IfStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}