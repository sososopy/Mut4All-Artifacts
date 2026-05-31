//source file
#include "../include/modify_destructor_defaulting_232.h"

// ========================================================================================================
#define MUT232_OUTPUT 1

void MutatorFrontendAction_232::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Dtor = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("DefaultedDestructor")) {
      //Filter nodes in header files
      if (!Dtor || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Dtor->getLocation()))
        return;
      
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 Dtor->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string newDtor = "~" + Dtor->getParent()->getNameAsString() + "() { /* mut232 */ }";
      declaration.replace(declaration.find("= default"), 9, newDtor);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Dtor->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_232::MutatorASTConsumer_232::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxDestructorDecl(isDefaulted()).bind("DefaultedDestructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}