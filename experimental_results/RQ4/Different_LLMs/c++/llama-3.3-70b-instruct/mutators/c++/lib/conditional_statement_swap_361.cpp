//source file
#include "../include/Mutator_Conditional_Statement_Swap_361.h"

// ========================================================================================================
#define MUT361_OUTPUT 1

void MutatorFrontendAction_361::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedDeclaration = declaration;
      size_t ifPos = declaration.find("if");
      size_t elsePos = declaration.find("else");
      if (ifPos != std::string::npos && elsePos != std::string::npos) {
        std::string condition = declaration.substr(ifPos + 3, elsePos - ifPos - 3);
        std::string thenStmt = declaration.substr(elsePos + 5, declaration.find("}", elsePos) - elsePos - 5);
        std::string elseStmt = declaration.substr(declaration.find("}", elsePos) + 1);
        mutatedDeclaration = "if (!(" + condition + ")) " + elseStmt + " else " + thenStmt;
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_361::MutatorASTConsumer_361::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = ifStmt().bind("IfStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}