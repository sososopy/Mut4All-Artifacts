//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mismatch_Nested_Generic_Lambda_48
 */ 
class MutatorFrontendAction_48 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(48)

private:
    class MutatorASTConsumer_48 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_48(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::LambdaExpr*> outerLambdas;
    };
};

//source file
#include "../include/Mutator_48.h"

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("outerLambda")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getLocation()))
        return;
      
      // Check if this lambda is generic (has template parameter list)
      auto *Method = LE->getCallOperator();
      if (!Method || !Method->getDescribedFunctionTemplate())