//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unknown_Parameter_In_Embed_Directive_227
 */ 
class MutatorFrontendAction_227 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(227)

private:
    class MutatorASTConsumer_227 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_227(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Unknown_Parameter_In_Embed_Directive_227.h"

// ========================================================================================================
#define MUT227_OUTPUT 1

void MutatorFrontendAction_227::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EmbedDirective>("EmbedDirective")) {
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getBeginLoc()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             ED->getSourceRange());
      auto params = ED->getParameters();
      if (!params.empty()) {
        auto param = params[0];
        if (!param.isKnown())
          return;
        auto param_content = stringutils::rangetoStr(
            *(Result.SourceManager), param.getSourceRange());
        llvm::outs() << param_content << '\n';
        if (param_content == "__FILE__")
          Rewrite.ReplaceText(ED->getBeginLoc(), 0, "/*mut227*/mut227 ");
        else
          Rewrite.ReplaceText(ED->getBeginLoc(), 0, "/*mut227*/mut227 ");
      } else {
        Rewrite.ReplaceText(ED->getBeginLoc(), 0, "/*mut227*/mut227 ");
      }
    }
}
  
void MutatorFrontendAction_227::MutatorASTConsumer_227::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = embedDirective().bind("EmbedDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}