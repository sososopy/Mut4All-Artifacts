//source file
#include "../include/Mutator_417.h"

// ========================================================================================================
#define MUT417_OUTPUT 1

void MutatorFrontendAction_417::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto name = MT->getNameAsString();
      if (name == "")
        return;
      auto definition =
          stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      llvm::outs() << "mut417:" << definition << '\n';
      definition = "template <typename T> " + definition;
      definition += "\n/*mut417*/template <typename T> void test() { " +
                    name + "(); }";
      llvm::outs() << "mut417:" << definition << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), definition);
    }
}
  
void MutatorFrontendAction_417::MutatorASTConsumer_417::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("Struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}