//header file
#pragma once
#include "Mutator_base.h"

/**
 * Static_Union_Init_301
 */ 
class MutatorFrontendAction_301 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(301)

private:
    class MutatorASTConsumer_301 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_301(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Static_Union_Init_301.h"

// ========================================================================================================
#define MUT301_OUTPUT 1

void MutatorFrontendAction_301::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("StaticUnion")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStaticLocal() == false)
        return;
      if (DL->getType().getTypePtr()->isUnionType() == false)
        return;
      if (DL->hasInit() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getInit()->getSourceRange());
      llvm::outs() << content;
      if (content.find('}') == string::npos)
        return;
      content.insert(content.find('}'), ".0");
      Rewrite.ReplaceText(DL->getInit()->getSourceRange(), content);
    }
}
  
void MutatorFrontendAction_301::MutatorASTConsumer_301::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl().bind("StaticUnion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}