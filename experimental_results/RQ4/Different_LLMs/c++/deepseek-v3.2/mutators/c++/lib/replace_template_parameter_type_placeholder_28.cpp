//source file
#include "../include/Mutator_Replace_Template_Parameter_Type_Placeholder_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("templateTypeParm")) {
      //Filter nodes in header files
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      //Get the source code text of target node
      auto sourceRange = TP->getSourceRange();
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      //Check if the template parameter has a preceding typename/class keyword
      bool hasKeyword = TP->wasDeclaredWithTypename();
      std::string paramName = TP->getNameAsString();
      //Generate a new identifier not used elsewhere
      char newId = 'z';
      while (usedIdentifiers.find(std::string(1, newId)) != usedIdentifiers.end() || newId < 'a') {
        newId--;
        if (newId < 'a') {
          newId = 'z';
          break;
        }
      }
      std::string newIdentifier = std::string(1, newId);
      usedIdentifiers.insert(newIdentifier);
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText;
      if (hasKeyword) {
        //Remove typename/class keyword
        mutatedText = newIdentifier;
      } else {
        //Already lacks keyword, replace with new identifier if needed
        if (paramName.empty() || usedIdentifiers.find(paramName) != usedIdentifiers.end()) {
          mutatedText = newIdentifier;
        } else {
          mutatedText = paramName;
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTypeParmDecl().bind("templateTypeParm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}