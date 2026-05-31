//source file
#include "../include/Remove_Constructor_Or_Destructor_425.h"

// ========================================================================================================
#define MUT425_OUTPUT 1

void MutatorFrontendAction_425::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isImplicit())
        return;
      if (MT->isCopyAssignmentOperator() || MT->isMoveAssignmentOperator())
        return;
      if (!MT->isUserProvided())
        return;
      if (!MT->getDescribedFunctionTemplate() &&
          !MT->getInstantiatedFromMemberFunction())
        return;
      if (!isa<CXXConstructorDecl>(MT) && !isa<CXXDestructorDecl>(MT))
        return;
      //Get the source code text of target node
      auto method = stringutils::rangetoStr(*(Result.SourceManager),
                                            MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      method = "/*mut425*/";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), method);
    }
}
  
void MutatorFrontendAction_425::MutatorASTConsumer_425::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl().bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}