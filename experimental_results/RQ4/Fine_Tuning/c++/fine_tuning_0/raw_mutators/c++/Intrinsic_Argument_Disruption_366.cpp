//header file
#pragma once
#include "Mutator_base.h"

/**
 * Intrinsic_Argument_Disruption_366
 */ 
class MutatorFrontendAction_366 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(366)

private:
    class MutatorASTConsumer_366 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_366(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Intrinsic_Argument_Disruption_366.h"

// ========================================================================================================
#define MUT366_OUTPUT 1

void MutatorFrontendAction_366::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("Intrinsics")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto callee = MT->getCallee();
      auto callee_name = stringutils::rangetoStr(*(Result.SourceManager),
                                                 callee->getSourceRange());
      llvm::outs() << callee_name << '\n';
      if (callee_name[0] != '_' || callee_name[1] != '_')
        return;
      auto args = MT->getArgs();
      if (args.size() == 0)
        return;
      auto arg0 = stringutils::rangetoStr(*(Result.SourceManager),
                                          args[0]->getSourceRange());
      llvm::outs() << arg0 << '\n';
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      int choice = getrandom::getRandomIndex(1);
      if (choice) {
        content = callee_name + "(" + arg0 + ")";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
      }
    }
}
  
void MutatorFrontendAction_366::MutatorASTConsumer_366::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr().bind("Intrinsics");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}