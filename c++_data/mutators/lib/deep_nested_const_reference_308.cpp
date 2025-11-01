//source file
#include "../include/deep_nested_const_reference_308.h"

// ========================================================================================================
#define MUT308_OUTPUT 1

void MutatorFrontendAction_308::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      auto type = VD->getType().getAsString();
      std::string nestedType = type;
      for (int i = 0; i < 20; ++i) {
          nestedType = "const " + nestedType + " &";
      }
      
      SourceLocation startLoc = VD->getBeginLoc();
      SourceLocation endLoc = VD->getTypeSourceInfo()->getTypeLoc().getEndLoc();
      Rewrite.ReplaceText(SourceRange(startLoc, endLoc), nestedType);
    }
}
  
void MutatorFrontendAction_308::MutatorASTConsumer_308::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(isInteger())).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}