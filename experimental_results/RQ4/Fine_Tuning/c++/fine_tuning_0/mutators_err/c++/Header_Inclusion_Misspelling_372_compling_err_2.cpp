//header file
#pragma once
#include "Mutator_base.h"

/**
 * Header_Inclusion_Misspelling_372
 */ 
class MutatorFrontendAction_372 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(372)

private:
    class MutatorASTConsumer_372 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_372(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Header_Inclusion_Misspelling_372.h"

// ========================================================================================================
#define MUT372_OUTPUT 1

void MutatorFrontendAction_372::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ID = Result.Nodes.getNodeAs<clang::InclusionDirective>("Inclusion")) {
      if (!ID || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ID->getBeginLoc()))
        return;
      auto header_name = ID->getFileName();
      llvm::outs() << header_name << '\n';
      if (header_name.size() == 0)
        return;
      int index = getrandom::getRandomIndex(header_name.size() - 1);
      header_name.erase(index, 1);
      llvm::outs() << header_name << '\n';
      Rewrite.ReplaceText(ID->getBeginLoc().getLocWithOffset(8 + index), 1, "");
    }
}
  
void MutatorFrontendAction_372::MutatorASTConsumer_372::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = inclusionDirective().bind("Inclusion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}