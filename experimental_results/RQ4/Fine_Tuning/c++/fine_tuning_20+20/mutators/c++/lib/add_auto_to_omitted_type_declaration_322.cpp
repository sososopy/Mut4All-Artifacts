//source file
#include "../include/Add_Auto_To_Omitted_Type_Declaration_322.h"

// ========================================================================================================
#define MUT322_OUTPUT 1

void MutatorFrontendAction_322::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("Decl")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      //Get the source code text of target node
      if (DL->getTypeSourceInfo() != nullptr)
        return;
      auto DL_str = stringutils::rangetoStr(*(Result.SourceManager),
                                            DL->getSourceRange());
      DL_str = "/*mut322*/auto " + DL_str;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(DL->getBeginLoc(), DL_str);
    }
}
  
void MutatorFrontendAction_322::MutatorASTConsumer_322::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl().bind("Decl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}