//source file
#include "../include/destructor_overload_mutation_590.h"

// ========================================================================================================
#define MUT590_OUTPUT 1

void MutatorFrontendAction_590::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DT = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Destructor")) {
      if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getLocation()))
        return;

      auto classDecl = DT->getParent();
      if (!classDecl || !classDecl->isCompleteDefinition())
        return;

      std::string className = classDecl->getNameAsString();
      std::string overloadText = "void ~" + className + "(int dummy) {}";
      SourceLocation insertLoc = classDecl->getEndLoc().getLocWithOffset(-1);
      Rewrite.InsertText(insertLoc, "\n/*mut590*/" + overloadText, true, true);
    }
}
  
void MutatorFrontendAction_590::MutatorASTConsumer_590::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxDestructorDecl(isUserProvided()).bind("Destructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}