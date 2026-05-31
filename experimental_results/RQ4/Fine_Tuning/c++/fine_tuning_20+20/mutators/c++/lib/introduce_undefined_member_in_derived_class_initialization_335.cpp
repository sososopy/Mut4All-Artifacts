//source file
#include "../include/Introduce_Undefined_Member_In_Derived_Class_Initialization_335.h"

// ========================================================================================================
#define MUT335_OUTPUT 1

void MutatorFrontendAction_335::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      //Get the source code text of target node
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Using) {
          return;
        }
      }
      if (DL->isCompleteDefinition() == false)
        return;
      auto bases = DL->bases();
      if (bases.empty())
        return;
      //Perform mutation on the source code text by applying string replacement
      string undefined_member = "undefined_member";
      undefined_member = "/*mut335*/" + undefined_member;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(DL->getEndLoc(), 0, undefined_member);
    }
}

void MutatorFrontendAction_335::MutatorASTConsumer_335::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}