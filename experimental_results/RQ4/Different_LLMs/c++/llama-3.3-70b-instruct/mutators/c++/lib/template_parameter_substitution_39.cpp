//source file
#include "../include/Mutator_Template_Parameter_Substitution_39.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/Decl.h"

// ========================================================================================================
#define MUT39_OUTPUT 1

void MutatorFrontendAction_39::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = Lexer::getSourceText(CharSourceRange::getTokenRange(MT->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      // Replace template parameter with different types or values
      std::string mutatedDeclaration = std::string(declaration);
      size_t templateParamPos = mutatedDeclaration.find('<');
      if (templateParamPos != std::string::npos) {
        size_t templateParamEndPos = mutatedDeclaration.find('>');
        if (templateParamEndPos != std::string::npos) {
          std::string templateParam = mutatedDeclaration.substr(templateParamPos + 1, templateParamEndPos - templateParamPos - 1);
          // Replace template parameter with different types or values
          if (templateParam == "int") {
            mutatedDeclaration.replace(templateParamPos + 1, templateParamEndPos - templateParamPos - 1, "float");
          } else if (templateParam == "float") {
            mutatedDeclaration.replace(templateParamPos + 1, templateParamEndPos - templateParamPos - 1, "char");
          } else {
            mutatedDeclaration.replace(templateParamPos + 1, templateParamEndPos - templateParamPos - 1, "int");
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_39::MutatorASTConsumer_39::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(hasDescendant(expr().bind("expr"))).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}