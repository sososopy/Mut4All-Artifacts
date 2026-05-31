//source file
#include "../include/Target_Function_Attribute_Mutation_401.h"

// ========================================================================================================
#define MUT401_OUTPUT 1

void MutatorFrontendAction_401::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Func")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->hasAttrs() == false)
        return;
      auto attrs = FD->getAttrs();
      for (auto attr : attrs) {
        auto range = attr->getRange();
        auto text =
            stringutils::rangetoStr(*(Result.SourceManager), range);
        if (text.find("target") != string::npos) {
          llvm::outs() << text << '\n';
          string new_text = "__attribute__((target(\"avx512f\")))";
          Rewrite.ReplaceText(range, new_text);
        }
      }
    }
}
  
void MutatorFrontendAction_401::MutatorASTConsumer_401::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("Func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}