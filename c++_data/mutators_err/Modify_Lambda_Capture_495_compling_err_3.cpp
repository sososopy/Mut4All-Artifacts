//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Capture_495
 */ 
class MutatorFrontendAction_495 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(495)

private:
    class MutatorASTConsumer_495 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_495(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Lambda_Capture_495.h"

// ========================================================================================================
#define MUT495_OUTPUT 1

void MutatorFrontendAction_495::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager),
                                                LE->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t captureStart = lambdaText.find('[');
      size_t captureEnd = lambdaText.find(']');
      if (captureStart != std::string::npos && captureEnd != std::string::npos) {
        std::string captureSection = lambdaText.substr(captureStart, captureEnd - captureStart + 1);
        std::string modifiedCaptureSection = "[=]"; // Change capture by reference to capture by value
        lambdaText.replace(captureStart, captureEnd - captureStart + 1, modifiedCaptureSection);

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambdaText);
      }
    }
}
  
void MutatorFrontendAction_495::MutatorASTConsumer_495::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr(hasCaptureDefault(capture_default::CD_ByRef)).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}