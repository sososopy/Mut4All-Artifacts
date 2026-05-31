//source file
#include "../include/Mutator_Destructor_Declaration_Inversion_In_Nested_Class_Context_172.h"

// ========================================================================================================
#define MUT172_OUTPUT 1

void MutatorFrontendAction_172::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Inner = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("InnerClass")) {
      //Filter nodes in header files
      if (!Inner || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Inner->getLocation()))
        return;
      if (!Inner->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto Dtor = Inner->getDestructor();
      bool hasUserDeclaredDtor = Dtor && !Dtor->isImplicit();
      
      if (hasUserDeclaredDtor) {
        // Case: remove existing destructor
        if (Dtor->getSourceRange().isValid()) {
          Rewrite.RemoveText(Dtor->getSourceRange());
        }
      } else {
        // Case: add defaulted destructor
        std::string DtorDecl = "~" + Inner->getNameAsString() + "() = default;";
        // Insert before the closing brace of the inner class
        SourceLocation InsertLoc = Inner->getEndLoc().getLocWithOffset(-1);
        Rewrite.InsertText(InsertLoc, "\n    /*mut172*/" + DtorDecl);
      }
    }
}
  
void MutatorFrontendAction_172::MutatorASTConsumer_172::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(
      hasParent(cxxRecordDecl()),
      unless(isImplicit())
    ).bind("InnerClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}