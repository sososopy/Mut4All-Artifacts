//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/SourceMgr.h"

/**
 * Replace_Lambda_Capture_With_Default_Capture_44
 */ 
class MutatorFrontendAction_44 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(44)

private:
    class MutatorASTConsumer_44 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_44(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Lambda_Capture_With_Default_Capture_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!LT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaText = llvm::StringRef::withNullAsEmpty(Result.SourceManager->getBuffer(LT->getSourceRange().getBegin().getLoc().getFileID())->getBuffer())->substr(LT->getSourceRange().getBegin().getOffset(), (LT->getSourceRange().getEnd().getOffset() - LT->getSourceRange().getBegin().getOffset()));
      //Perform mutation on the source code text by applying string replacement
      if (LT->captureDefault() == clang::LambdaCaptureDefault::CD_None) {
        lambdaText = "[=]" + lambdaText.substr(lambdaText.find("]")+1);
      } else if (LT->captureDefault() == clang::LambdaCaptureDefault::CD_ByValue) {
        lambdaText = "[&]" + lambdaText.substr(lambdaText.find("]")+1);
      } else if (LT->captureDefault() == clang::LambdaCaptureDefault::CD_ByReference) {
        lambdaText = "[=]" + lambdaText.substr(lambdaText.find("]")+1);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(LT->getSourceRange(), lambdaText);
    }
}
  
void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}