//source file
#include "../include/insert_unreachable_code_block_460.h"

// ========================================================================================================
#define MUT460_OUTPUT 1

void MutatorFrontendAction_460::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        auto body = FD->getBody();
        auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());

        std::string unreachableCode = "\n/*mut460*/if (false) { while(true) {}; }\n";
        bodyText.insert(bodyText.find("{") + 1, unreachableCode);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
      }
    }
}
  
void MutatorFrontendAction_460::MutatorASTConsumer_460::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("FuncDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}