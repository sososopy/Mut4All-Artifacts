//source file
#include "../include/misuse_embed_directive_553.h"

// ========================================================================================================
#define MUT553_OUTPUT 1

void MutatorFrontendAction_553::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SL = Result.Nodes.getNodeAs<clang::StringLiteral>("embedString")) {
      if (!SL || !Result.SourceManager->isWrittenInMainFile(SL->getBeginLoc()))
        return;

      std::string originalEmbed = SL->getString().str();
      std::string mutatedEmbed = originalEmbed;

      size_t prefixPos = mutatedEmbed.find("prefix(");
      if (prefixPos != std::string::npos) {
        size_t prefixEnd = mutatedEmbed.find(")", prefixPos);
        if (prefixEnd != std::string::npos) {
          mutatedEmbed.insert(prefixEnd, ", 'z'");
        }
      }

      size_t limitPos = mutatedEmbed.find("limit(");
      if (limitPos != std::string::npos) {
        size_t limitEnd = mutatedEmbed.find(")", limitPos);
        if (limitEnd != std::string::npos) {
          mutatedEmbed.replace(limitPos, limitEnd - limitPos + 1, "limit(0xFFFFFFFF)");
        }
      }

      if (mutatedEmbed != originalEmbed) {
        Rewrite.ReplaceText(SL->getSourceRange(), mutatedEmbed);
      }
    }
}
  
void MutatorFrontendAction_553::MutatorASTConsumer_553::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = stringLiteral().bind("embedString");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}