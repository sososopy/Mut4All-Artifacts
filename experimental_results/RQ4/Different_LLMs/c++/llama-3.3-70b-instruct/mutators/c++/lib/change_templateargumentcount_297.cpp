//source file
#include "../include/Change_TemplateArgumentCount_297.h"

// ========================================================================================================
#define MUT297_OUTPUT 1

void MutatorFrontendAction_297::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Add or remove template arguments
      int numArgs = MT->getTemplateParameters()->size();
      if (numArgs > 0) {
        // Remove a template argument
        std::string newDeclaration = declaration;
        size_t pos = newDeclaration.find("<");
        size_t endPos = newDeclaration.find(">");
        std::string args = newDeclaration.substr(pos + 1, endPos - pos - 1);
        std::vector<std::string> argList;
        size_t argPos = 0;
        while ((argPos = args.find(",")) != std::string::npos) {
          argList.push_back(args.substr(0, argPos));
          args.erase(0, argPos + 1);
        }
        argList.push_back(args);
        if (argList.size() > 1) {
          argList.pop_back();
          std::string newArgs;
          for (const auto &arg : argList) {
            newArgs += arg + ",";
          }
          newArgs.pop_back(); // Remove the last comma
          newDeclaration.replace(pos + 1, endPos - pos - 1, newArgs);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), newDeclaration);
        }
      } else {
        // Add a template argument
        std::string newDeclaration = declaration;
        size_t pos = newDeclaration.find("<");
        size_t endPos = newDeclaration.find(">");
        std::string args = newDeclaration.substr(pos + 1, endPos - pos - 1);
        std::string newArgs = args + ", typename T3";
        newDeclaration.replace(pos + 1, endPos - pos - 1, newArgs);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), newDeclaration);
      }
    }
}
  
void MutatorFrontendAction_297::MutatorASTConsumer_297::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}