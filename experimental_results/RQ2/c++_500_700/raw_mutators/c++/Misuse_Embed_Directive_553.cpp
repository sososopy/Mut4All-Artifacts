//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_embed_directive_553
 */ 
class MutatorFrontendAction_553 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(553)

private:
    class MutatorASTConsumer_553 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_553(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/misuse_embed_directive_553.h"

// ========================================================================================================
#define MUT553_OUTPUT 1

void MutatorFrontendAction_553::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SL = Result.Nodes.getNodeAs<clang::StringLiteral>("embedString")) {
      if (!SL || !Result.Context->getSourceManager().isWrittenInMainFile(SL->getLocation()))
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
    auto matcher = stringLiteral(hasSubstring("#embed")).bind("embedString");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}