//source file
#include "../include/Modify_Using_Declarations_In_Derived_Classes_97.h"

// ========================================================================================================
#define MUT97_OUTPUT 1

void MutatorFrontendAction_97::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UD = Result.Nodes.getNodeAs<clang::UsingDecl>("UsingDecl")) {
      //Filter nodes in header files
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;

      //Get the source code text of target node
      auto usingText = stringutils::rangetoStr(*(Result.SourceManager), UD->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      size_t pos = usingText.find("::");
      if (pos != std::string::npos) {
          usingText = usingText.substr(0, pos + 2) + ";";
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(UD->getSourceRange()), usingText);
    }
}
  
void MutatorFrontendAction_97::MutatorASTConsumer_97::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = usingDecl(hasParent(cxxRecordDecl(isDerivedFrom(anything())))).bind("UsingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}