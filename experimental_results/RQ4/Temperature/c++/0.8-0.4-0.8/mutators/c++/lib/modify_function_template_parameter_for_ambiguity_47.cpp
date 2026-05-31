//source file
#include "../include/modify_function_template_parameter_for_ambiguity_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto functionDecl = FD->getTemplatedDecl();
      if (!functionDecl)
        return;

      std::string functionText = stringutils::rangetoStr(*(Result.SourceManager), functionDecl->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      for (auto *param : functionDecl->parameters()) {
          std::string paramType = param->getType().getAsString();
          if (!paramType.empty()) {
              std::string newParamType = paramType + "*"; // Change to pointer type
              size_t pos = functionText.find(paramType);
              if (pos != std::string::npos) {
                  functionText.replace(pos, paramType.length(), newParamType);
              }
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(functionDecl->getSourceRange()), functionText);
    }
}
  
void MutatorFrontendAction_47::MutatorASTConsumer_47::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(
        hasDescendant(functionDecl().bind("FuncTemplate"))
    );
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}