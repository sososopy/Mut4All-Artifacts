//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceLambdaWithFunctionPointer_78
 */ 
class MutatorFrontendAction_78 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(78)

private:
    class MutatorASTConsumer_78 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_78(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ReplaceLambdaWithFunctionPointer_78.h"

// ========================================================================================================
#define MUT78_OUTPUT 1

void MutatorFrontendAction_78::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Call = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!Call || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Call->getLocStart()))
        return;
      //Get the source code text of target node
      auto args = Call->args();
      for (auto arg : args) {
        if (auto *Lambda = dyn_cast<clang::LambdaExpr>(arg)) {
          //Perform mutation on the source code text by applying string replacement
          std::string funcName = "lambda_func_" + std::to_string(getrandom::getRandomIndex(1000));
          std::string funcDecl = "auto " + funcName + " = " + stringutils::rangetoStr(*(Result.SourceManager), Lambda->getSourceRange()) + ";";
          std::string funcPtrDecl = funcName;
          //Replace the original AST node with the mutated one
          Rewrite.InsertTextBefore(Call->getLocStart(), funcDecl + "\n");
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(arg->getSourceRange()), funcPtrDecl);
        }
      }
    }
}
  
void MutatorFrontendAction_78::MutatorASTConsumer_78::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    CallExprMatcher matcher = callExpr(hasArgument(0, lambdaExpr())).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}