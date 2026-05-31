//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_with_Parameter_Pack_in_Decltype_126
 */ 
class MutatorFrontendAction_126 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(126)

private:
    class MutatorASTConsumer_126 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_126(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Lambda_with_Parameter_Pack_in_Decltype_126.h"

// ========================================================================================================
#define MUT126_OUTPUT 1

void MutatorFrontendAction_126::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DT = Result.Nodes.getNodeAs<clang::DecltypeType>("Decltype")) {
      if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getBeginLoc()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DT->getBeginLoc());
      llvm::outs() << content << '\n';
      auto lambda_with_parameter_pack =
          "/*mut126*/([]<typename... T>(T&&... args){ return (args + ...); }(5, "
          "10))";
      content = lambda_with_parameter_pack;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DT->getBeginLoc()), content);
    }
}
  
void MutatorFrontendAction_126::MutatorASTConsumer_126::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = decltypeType().bind("Decltype");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}