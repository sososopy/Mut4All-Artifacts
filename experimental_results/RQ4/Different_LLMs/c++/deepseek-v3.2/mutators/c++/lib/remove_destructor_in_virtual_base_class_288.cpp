//source file
#include "../include/Mutator_288.h"

// ========================================================================================================
#define MUT288_OUTPUT 1

void MutatorFrontendAction_288::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassWithVirtualBase")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      //Check if class has virtual base
      bool hasVirtualBase = false;
      for (auto base : MT->bases()) {
        if (base.isVirtual()) {
          hasVirtualBase = true;
          break;
        }
      }
      if (!hasVirtualBase)
        return;
      //Get destructor
      auto dtor = MT->getDestructor();
      if (!dtor || dtor->isImplicit())
        return;
      //Get the source code text of target node
      auto dtorRange = dtor->getSourceRange();
      if (dtorRange.isInvalid())
        return;
      //Perform mutation on the source code text by applying string replacement
      //Remove the destructor declaration by deleting its source range
      //Replace the original AST node with the mutated one
      Rewrite.RemoveText(dtorRange);
    }
}
  
void MutatorFrontendAction_288::MutatorASTConsumer_288::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasDefinition(), hasMethod(hasName("~"))).bind("ClassWithVirtualBase");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}