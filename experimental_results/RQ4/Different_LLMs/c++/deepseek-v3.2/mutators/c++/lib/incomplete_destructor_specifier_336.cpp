//source file
#include "../include/Mutator_Incomplete_Destructor_Specifier_336.h"

// ========================================================================================================
#define MUT336_OUTPUT 1

void MutatorFrontendAction_336::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DT = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Destructor")) {
      //Filter nodes in header files
      if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getLocation()))
        return;
      //Only mutate once per seed program
      if (mutated) return;
      //Only mutate user-defined destructors (not implicit)
      if (DT->isImplicit()) return;
      //Get the source code text of target node
      auto destructorText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   DT->getSourceRange());
      //Find the class name after the tilde
      size_t tildePos = destructorText.find('~');
      if (tildePos == std::string::npos) return;
      size_t parenPos = destructorText.find('(', tildePos);
      if (parenPos == std::string::npos) return;
      //Extract the class name between ~ and (
      std::string className = destructorText.substr(tildePos + 1, parenPos - tildePos - 1);
      //Perform mutation on the source code text by applying string replacement
      //Remove the class name, leaving only ~()
      destructorText.replace(tildePos + 1, className.length(), "");
      destructorText = "/*mut336*/" + destructorText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DT->getSourceRange()), destructorText);
      mutated = true;
    }
}
  
void MutatorFrontendAction_336::MutatorASTConsumer_336::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxDestructorDecl().bind("Destructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}