//source file
#include "../include/modify_function_template_with_parameter_pack_491.h"

// ========================================================================================================
#define MUT491_OUTPUT 1

void MutatorFrontendAction_491::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      // Record the original function template if not already recorded
      if (!OriginalTemplate) {
          OriginalTemplate = FTD;
      }

      // Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FTD->getSourceRange());
      
      // Perform mutation on the source code text by applying string replacement
      // Modify the function template to include a parameter pack
      if (OriginalTemplate == FTD) {
          std::string newDeclaration = declaration;
          size_t pos = newDeclaration.find(")");
          if (pos != std::string::npos) {
              newDeclaration.insert(pos, ", typename... Args");
          }
          pos = newDeclaration.find("{");
          if (pos != std::string::npos) {
              newDeclaration.insert(pos + 1, "\n/*mut491*/");
          }
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), newDeclaration);
      } else {
          // Define an additional function template overload
          std::string overloadDeclaration = declaration;
          size_t pos = overloadDeclaration.find(")");
          if (pos != std::string::npos) {
              overloadDeclaration.insert(pos, ", Args... args");
          }
          pos = overloadDeclaration.find("{");
          if (pos != std::string::npos) {
              overloadDeclaration.insert(pos + 1, "\n/*mut491 overload*/");
          }
          Rewrite.InsertTextAfterToken(FTD->getEndLoc(), "\n" + overloadDeclaration);
      }
    }
}
  
void MutatorFrontendAction_491::MutatorASTConsumer_491::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}