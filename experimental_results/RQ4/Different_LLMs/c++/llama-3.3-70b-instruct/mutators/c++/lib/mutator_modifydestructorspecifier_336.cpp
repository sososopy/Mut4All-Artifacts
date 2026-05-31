//source file
#include "../include/Mutator_ModifyDestructorSpecifier_336.h"

// ========================================================================================================
#define MUT336_OUTPUT 1

void MutatorFrontendAction_336::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Destructor")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find('~');
      if (pos != string::npos) {
        declaration.replace(pos, 1, "!");
      } else {
        declaration = "!" + declaration;
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Class")) {
      //Filter nodes in header files
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      //Get the source code text of target node
      auto classDeclaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   CL->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      classDeclaration += "\n!~" + CL->getNameAsString() + "();";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()), classDeclaration);
    }
}
  
void MutatorFrontendAction_336::MutatorASTConsumer_336::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = cxxDestructorDecl().bind("Destructor");
    DeclarationMatcher matcher2 = cxxRecordDecl().bind("Class");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}