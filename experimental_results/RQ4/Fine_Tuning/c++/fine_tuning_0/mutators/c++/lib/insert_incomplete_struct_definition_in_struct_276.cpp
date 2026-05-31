//source file
#include "../include/Insert_Incomplete_Struct_Definition_In_Struct_276.h"

// ========================================================================================================
#define MUT276_OUTPUT 1

void MutatorFrontendAction_276::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;
      if (!SD->isStruct())
        return;
      if (!SD->isCompleteDefinition())
        return;
      auto name = SD->getNameAsString();
      auto insertion = "struct " + name + ";";
      llvm::outs() << insertion << "\n";
      Rewrite.ReplaceText(SD->getBeginLoc(), 0, "/*mut276*/"+insertion);
    }
}
  
void MutatorFrontendAction_276::MutatorASTConsumer_276::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}