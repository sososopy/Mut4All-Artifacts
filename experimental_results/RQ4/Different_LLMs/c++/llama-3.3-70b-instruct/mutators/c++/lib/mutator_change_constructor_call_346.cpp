//source file
#include "../include/Mutator_Change_Constructor_Call_346.h"

// ========================================================================================================
#define MUT346_OUTPUT 1

void MutatorFrontendAction_346::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("ConstructorCall")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Change the number or types of arguments passed to the constructor
      std::string mutatedDeclaration = declaration;
      std::size_t openParenthesisIndex = declaration.find('(');
      std::size_t closeParenthesisIndex = declaration.find(')');
      std::string arguments = declaration.substr(openParenthesisIndex + 1, closeParenthesisIndex - openParenthesisIndex - 1);
      std::vector<std::string> args;
      std::size_t commaIndex = arguments.find(',');
      while (commaIndex != std::string::npos) {
        args.push_back(arguments.substr(0, commaIndex));
        arguments = arguments.substr(commaIndex + 1);
        commaIndex = arguments.find(',');
      }
      args.push_back(arguments);
      // Randomly remove or add an argument
      if (args.size() > 1) {
        // Remove an argument
        mutatedDeclaration = declaration.substr(0, openParenthesisIndex + 1);
        for (std::size_t i = 0; i < args.size() - 1; ++i) {
          mutatedDeclaration += args[i] + ",";
        }
        mutatedDeclaration += args[args.size() - 1] + declaration.substr(closeParenthesisIndex);
      } else if (args.size() == 1) {
        // Add an argument
        mutatedDeclaration = declaration.substr(0, closeParenthesisIndex) + ", 0" + declaration.substr(closeParenthesisIndex);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_346::MutatorASTConsumer_346::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxConstructExpr().bind("ConstructorCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}