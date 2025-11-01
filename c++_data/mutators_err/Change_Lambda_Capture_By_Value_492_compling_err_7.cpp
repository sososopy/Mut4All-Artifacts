//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

/**
 * Change_Lambda_Capture_By_Value_492
 */ 
class MutatorFrontendAction_492 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(492)

private:
    class MutatorASTConsumer_492 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_492(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/change_lambda_capture_by_value_492.h"

// ========================================================================================================
#define MUT492_OUTPUT 1

void MutatorFrontendAction_492::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                  LE->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      auto captureStart = lambdaSource.find("[&");
      if (captureStart != std::string::npos) {
          lambdaSource.replace(captureStart, 2, "[=");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_492::MutatorASTConsumer_492::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasCaptureDefault(capture_default_is(clang::LCD_ByRef))).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}