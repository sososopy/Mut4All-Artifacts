//source file
#include "../include/Malformed_Array_Subscript_264.h"

// ========================================================================================================
#define MUT264_OUTPUT 1

void MutatorFrontendAction_264::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record array variable names
      if (VD->getType()->isArrayType()) {
        arrayNames.push_back(VD->getNameAsString());
      }
    }
    else if (auto *AS = Result.Nodes.getNodeAs<clang::ArraySubscriptExpr>("arraySub")) {
      //Filter nodes in header files
      if (!AS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AS->getBeginLoc()))
        return;
      //Get the source code text of target node
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), AS->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText;
      int choice = getrandom::getRandomIndex(2);
      if (choice == 0) {
        // Replace with empty subscript: arr[]
        size_t pos = originalText.find('[');
        if (pos != std::string::npos) {
          size_t endPos = originalText.find(']', pos);
          if (endPos != std::string::npos) {
            mutatedText = originalText.substr(0, pos+1) + "]";
          }
        }
      } else if (choice == 1) {
        // Replace with extra bracket: arr[][]
        size_t pos = originalText.find('[');
        if (pos != std::string::npos) {
          size_t endPos = originalText.find(']', pos);
          if (endPos != std::string::npos) {
            mutatedText = originalText.substr(0, pos+1) + "][]";
          }
        }
      } else {
        // Introduce local array and apply mutation
        if (arrayNames.empty()) {
          // No array found, create local array declaration and malformed subscript
          std::string localArray = "int localArr[5];\n";
          std::string malformedUse;
          int subChoice = getrandom::getRandomIndex(1);
          if (subChoice == 0) {
            malformedUse = "localArr[]";
          } else {
            malformedUse = "localArr[][]";
          }
          mutatedText = localArray + malformedUse;
        } else {
          // Use existing array name with malformed subscript
          size_t idx = getrandom::getRandomIndex(arrayNames.size()-1);
          std::string arrName = arrayNames[idx];
          int subChoice = getrandom::getRandomIndex(1);
          if (subChoice == 0) {
            mutatedText = arrName + "[]";
          } else {
            mutatedText = arrName + "[][]";
          }
        }
      }
      mutatedText = "/*mut264*/" + mutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(AS->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_264::MutatorASTConsumer_264::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher arrayMatcher = varDecl(hasType(arrayType())).bind("arrayVar");
    StatementMatcher subscriptMatcher = arraySubscriptExpr().bind("arraySub");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(arrayMatcher, &callback);
    matchFinder.addMatcher(subscriptMatcher, &callback);
    matchFinder.matchAST(Context);
}