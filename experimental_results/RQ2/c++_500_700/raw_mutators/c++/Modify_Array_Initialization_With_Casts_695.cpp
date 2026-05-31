//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_array_initialization_with_casts_695
 */ 
class MutatorFrontendAction_695 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(695)

private:
    class MutatorASTConsumer_695 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_695(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_array_initialization_with_casts_695.h"

// ========================================================================================================
#define MUT695_OUTPUT 1

void MutatorFrontendAction_695::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *initExpr = Result.Nodes.getNodeAs<clang::CStyleCastExpr>("castExpr")) {
      //Filter nodes in header files
      if (!initExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     initExpr->getExprLoc()))
        return;
      
      // Check if the cast type is (int *)
      if (initExpr->getTypeAsWritten()->isPointerType()) {
        auto pointeeType = initExpr->getTypeAsWritten()->getPointeeType();
        if (pointeeType->isIntegerType()) {
          //Perform mutation on the source code text by applying string replacement
          auto castRange = initExpr->getTypeInfoAsWritten()->getTypeLoc().getSourceRange();
          Rewrite.ReplaceText(castRange, "void *");
        }
      }
    }
}
  
void MutatorFrontendAction_695::MutatorASTConsumer_695::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cStyleCastExpr(hasDestinationType(pointerType(pointee(isInteger())))).bind("castExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}