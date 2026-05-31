//source file
#include "../include/Modify_Function_Parameter_List_219.h"

// ========================================================================================================
#define MUT219_OUTPUT 1

void MutatorFrontendAction_219::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::vector<std::string> params;
      for (auto param : FD->parameters()) {
        params.push_back(param->getOriginalType().getAsString() + " " + param->getNameAsString());
      }
      int choice = getrandom::getRandomIndex(3);
      if (choice == 0) { // Add a new parameter
        std::string newParamType = "int"; // Replace with actual type
        std::string newParamName = "newParam"; // Replace with actual name
        params.push_back(newParamType + " " + newParamName);
      } else if (choice == 1) { // Remove a parameter
        if (!params.empty()) {
          params.erase(params.begin() + getrandom::getRandomIndex(params.size()));
        }
      } else { // Modify an existing parameter
        if (!params.empty()) {
          int index = getrandom::getRandomIndex(params.size());
          std::string newParamType = "float"; // Replace with actual type
          params[index] = newParamType + " " + params[index].substr(params[index].find(' ') + 1);
        }
      }
      std::string mutatedParams;
      for (auto param : params) {
        mutatedParams += param + ", ";
      }
      if (!mutatedParams.empty()) {
        mutatedParams.pop_back();
        mutatedParams.pop_back();
      }
      std::string mutatedDeclaration = declaration;
      size_t pos = mutatedDeclaration.find('(');
      mutatedDeclaration.replace(pos + 1, declaration.find(')') - pos - 1, mutatedParams);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_219::MutatorASTConsumer_219::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}