//source file
#include "../include/Implicit_Member_Self_Reference_In_Dependent_Context_Initializer_5.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("fieldDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      if (!FD->hasInClassInitializer())
        return;
      Expr *InitExpr = FD->getInClassInitializer();
      if (!InitExpr)
        return;
      //Check if initializer is already self-referential
      if (auto *DRE = dyn_cast<DeclRefExpr>(InitExpr)) {
        if (DRE->getDecl() == FD)
          return;
      }
      //Perform mutation on the source code text by applying string replacement
      string MemberName = FD->getNameAsString();
      string Replacement = MemberName;
      SourceRange InitRange = InitExpr->getSourceRange();
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(InitRange, Replacement);
    }
}
  
void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(hasInClassInitializer(unless(hasDescendant(declRefExpr(to(fieldDecl())))))).bind("fieldDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}