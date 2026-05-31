//header file
#pragma once
#include "Mutator_base.h"

/**
 * Toggle_Consteval_User_Defined_Conversion_84
 */ 
class MutatorFrontendAction_84 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(84)

private:
    class MutatorASTConsumer_84 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_84(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Toggle_Consteval_User_Defined_Conversion_84.h"

// ========================================================================================================
#define MUT84_OUTPUT 1

void MutatorFrontendAction_84::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXConversionDecl>("Conversions")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (DL->isConsteval()) {
        llvm::outs() << "/*mut84*/" << content << '\n';
        Rewrite.ReplaceText(DL->getBeginLoc(), 9, "");
      } else {
        Rewrite.ReplaceText(DL->getBeginLoc(), 0, "/*mut84*/consteval ");
      }
    }
}
  
void MutatorFrontendAction_84::MutatorASTConsumer_84::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxConversionDecl().bind("Conversions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}