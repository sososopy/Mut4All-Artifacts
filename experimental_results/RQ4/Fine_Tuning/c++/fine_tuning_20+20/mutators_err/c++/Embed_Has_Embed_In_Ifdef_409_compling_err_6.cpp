//header file
#pragma once
#include "Mutator_base.h"

/**
 * Embed_Has_Embed_In_Ifdef_409
 */ 
class MutatorFrontendAction_409 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(409)

private:
    class MutatorASTConsumer_409 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_409(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        
    };
};

//source file
#include "../include/embed_has_embed_in_ifdef_409.h"

// ========================================================================================================
#define MUT409_OUTPUT 1

void MutatorFrontendAction_409::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::IfDirective>("Ifdef")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      content = "#if __has_embed(__FILE__)\n";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_409::MutatorASTConsumer_409::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = ifDirective().bind("Ifdef");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}