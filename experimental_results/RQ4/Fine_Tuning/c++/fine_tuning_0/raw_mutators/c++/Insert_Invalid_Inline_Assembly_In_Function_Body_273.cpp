//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Inline_Assembly_In_Function_Body_273
 */ 
class MutatorFrontendAction_273 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(273)

private:
    class MutatorASTConsumer_273 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_273(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Invalid_Inline_Assembly_In_Function_Body_273.h"

// ========================================================================================================
#define MUT273_OUTPUT 1

void MutatorFrontendAction_273::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      auto body = FD->getBody();
      auto body_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                  body->getSourceRange());
      llvm::outs() << body_content << '\n';
      body_content.insert(body_content.find('{') + 1,
                          "asm(\"invalid_instruction\");");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()),
                          body_content);
    }
}
  
void MutatorFrontendAction_273::MutatorASTConsumer_273::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}