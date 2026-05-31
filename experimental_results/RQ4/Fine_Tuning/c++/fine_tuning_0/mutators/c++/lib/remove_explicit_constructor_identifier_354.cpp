//source file
#include "../include/Remove_Explicit_Constructor_Identifier_354.h"

// ========================================================================================================
#define MUT354_OUTPUT 1

void MutatorFrontendAction_354::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructors")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isImplicit())
        return;
      if (MT->isDefaulted())
        return;
      auto class_name = MT->getParent()->getNameAsString();
      auto constructor_name = MT->getNameAsString();
      if (class_name != constructor_name)
        return;
      auto constructor_def = stringutils::rangetoStr(
          *(Result.SourceManager), MT->getSourceRange());
      llvm::outs() << constructor_def << '\n';
      auto pos = constructor_def.find(class_name);
      if (pos == string::npos)
        return;
      constructor_def.replace(pos, class_name.length(), "");
      llvm::outs() << constructor_def << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          constructor_def);
    }
}
  
void MutatorFrontendAction_354::MutatorASTConsumer_354::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Constructors");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}