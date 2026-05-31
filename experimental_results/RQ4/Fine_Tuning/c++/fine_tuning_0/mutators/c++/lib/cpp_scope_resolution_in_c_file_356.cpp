//source file
#include "../include/Cpp_Scope_Resolution_In_C_File_356.h"

// ========================================================================================================
#define MUT356_OUTPUT 1

void MutatorFrontendAction_356::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("MainFile")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto filename = Result.SourceManager->getFilename(MT->getBeginLoc());
      if (filename.endswith(".c") == false)
        return;
      auto fileID = Result.SourceManager->getMainFileID();
      auto fileEntry = Result.SourceManager->getFileEntryForID(fileID);
      if (!fileEntry)
        return;
      llvm::outs() << "/*mut356*/::";
      Rewrite.ReplaceText(MT->getBeginLoc(), 0, "::");
    }
}
  
void MutatorFrontendAction_356::MutatorASTConsumer_356::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = translationUnitDecl().bind("MainFile");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}