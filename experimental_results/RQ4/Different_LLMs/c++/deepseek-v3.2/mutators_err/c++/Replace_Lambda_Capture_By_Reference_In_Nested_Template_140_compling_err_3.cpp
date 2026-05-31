//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Capture_By_Reference_In_Nested_Template_140
 */ 
class MutatorFrontendAction_140 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(140)

private:
    class MutatorASTConsumer_140 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_140(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Replace_Lambda_Capture_By_Reference_In_Nested_Template_140.h"

// ========================================================================================================
#define MUT140_OUTPUT 1

void MutatorFrontendAction_140::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaRange = MT->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      //Check if the lambda has any explicit captures
      auto captures = MT->captures();
      bool hasExplicitRefCapture = false;
      for (auto &capture : captures) {
        if (capture.isExplicit() && capture.getCaptureKind() == clang::LCK_ByRef) {
          hasExplicitRefCapture = true;
          break;
        }
      }
      if (!hasExplicitRefCapture) return;
      //Perform mutation on the source code text by applying string replacement
      //Find the capture list and replace '&' with nothing for the first explicit reference capture
      size_t captureStart = lambdaText.find('[');
      size_t captureEnd = lambdaText.find(']');
      if (captureStart == std::string::npos || captureEnd == std::string::npos) return;
      std::string captureList = lambdaText.substr(captureStart + 1, captureEnd - captureStart - 1);
      //Find the first explicit reference capture pattern (like '&var')
      size_t ampPos = captureList.find('&');
      if (ampPos == std::string::npos) return;
      //Check if this '&' is part of an explicit capture (not part of '&=' or '[&]')
      if (ampPos > 0 && captureList[ampPos-1] == '=') return; // skip '&='
      if (ampPos == 0 && captureList.size() > 1 && captureList[1] == ']') return; // skip '[&]' default capture
      //Remove the '&' character
      captureList.erase(ampPos, 1);
      std::string mutatedLambda = lambdaText.substr(0, captureStart + 1) + captureList + lambdaText.substr(captureEnd);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), mutatedLambda);
    }
}
  
void MutatorFrontendAction_140::MutatorASTConsumer_140::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}