//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Structured_Binding_Declaration_In_Lambda_Capture_344
 */ 
class MutatorFrontendAction_344 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(344)

private:
    class MutatorASTConsumer_344 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_344(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Modify_Structured_Binding_Declaration_In_Lambda_Capture_344.h"

// ========================================================================================================
#define MUT344_OUTPUT 1

void MutatorFrontendAction_344::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>(("Lambda"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc().getLocWithOffset(0)))
        return;
      //Get the source code text of target node
      auto capture = MT->getCaptures().begin();
      if (capture != MT->getCaptures().end() && (*capture)->isInitCapture()) {
        auto init = (*capture)->getInit();
        if (auto declRef = dyn_cast<DeclRefExpr>(init)) {
          auto decl = declRef->getDecl();
          if (auto varDecl = dyn_cast<VarDecl>(decl)) {
            if (varDecl->isStructuredBinding()) {
              //Perform mutation on the source code text by applying string replacement
              auto varName = varDecl->getNameAsString();
              auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
              lambdaText = lambdaText.replace(lambdaText.find("[" + varName + "]"), varName.length() + 2, varName);
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), lambdaText);
            }
          }
        }
      }
    }
}

void MutatorFrontendAction_344::MutatorASTConsumer_344::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}