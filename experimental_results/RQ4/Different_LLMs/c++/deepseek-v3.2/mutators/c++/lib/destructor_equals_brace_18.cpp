//source file
#include "../include/Mutator_18.h"

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DT = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Destructor")) {
      //Filter nodes in header files
      if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getLocation()))
        return;
      //Get the source code text of target node
      auto destructorText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   DT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (destructorText.find("= {}") != std::string::npos) {
        int choice = getrandom::getRandomIndex(2);
        std::string mutatedText;
        if (choice == 0) {
          mutatedText = destructorText.replace(destructorText.find("= {}"), 4, "= {;}");
        } else {
          mutatedText = destructorText.replace(destructorText.find("= {}"), 4, "= { }");
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(DT->getSourceRange()), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxDestructorDecl().bind("Destructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}