//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Default_Arguments_In_Generic_Lambdas_426
 */ 
class MutatorFrontendAction_426 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(426)

private:
    class MutatorASTConsumer_426 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_426(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Default_Arguments_In_Generic_Lambdas_426.h"

// ========================================================================================================
#define MUT426_OUTPUT 1

void MutatorFrontendAction_426::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("GenericLambda")) {
      //Filter nodes in header files
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto CaptureDefault = LambdaExpr->getCaptureDefault();
      if (CaptureDefault != clang::LCD_None)
        return;

      for (auto *Param : LambdaExpr->getLambdaClass()->getLambdaCallOperator()->parameters()) {
        if (auto *ParmVar = llvm::dyn_cast<clang::ParmVarDecl>(Param)) {
          if (ParmVar->hasDefaultArg()) {
            auto DefaultArg = ParmVar->getDefaultArg();
            if (DefaultArg->getStmtClass() == clang::Stmt::IntegerLiteralClass) {
              //Perform mutation on the source code text by applying string replacement
              auto DefaultArgStr = "\"default\"";
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(DefaultArg->getSourceRange(), DefaultArgStr);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_426::MutatorASTConsumer_426::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasParameter(anyOf(
        parmVarDecl(hasType(autoType()), hasDefaultArgument())))).bind("GenericLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}