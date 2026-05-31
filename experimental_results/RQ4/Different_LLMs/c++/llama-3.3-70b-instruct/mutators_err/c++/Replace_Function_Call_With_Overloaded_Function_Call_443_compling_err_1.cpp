```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Call_With_Overloaded_Function_Call_443
 */ 
class MutatorFrontendAction_443 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(443)

private:
    class MutatorASTConsumer_443 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_443(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Function_Call_With_Overloaded_Function_Call_443.h"

// ========================================================================================================
#define MUT443_OUTPUT 1

void MutatorFrontendAction_443::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CallExpr->getLocStart()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   CallExpr->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Find overloaded functions with the same name
      auto funcDecl = CallExpr->getDirectCallee();
      if (!funcDecl)
        return;
      auto funcName = funcDecl->getNameAsString();
      auto overloadedFuncs = Result.Context->getTranslationUnitDecl()
          ->lookupSingleName(TU, DeclarationName(funcName));
      if (!overloadedFuncs)
        return;
      for (auto overloadedFunc : overloadedFuncs) {
        if (auto overloadedFuncDecl = dyn_cast<FunctionDecl>(overloadedFunc)) {
          if (overloadedFuncDecl->getNumParams() != funcDecl->getNumParams())
            continue;
          // Replace the original function call with the overloaded function call
          auto newCallExpr = "/*mut443*/" + funcName + "(";
          for (auto arg : CallExpr->arguments()) {
            newCallExpr += stringutils::rangetoStr(*(Result.SourceManager),
                                                    arg->getSourceRange()) + ", ";
          }
          newCallExpr.pop_back();
          newCallExpr.pop_back();
          newCallExpr += ")";
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(CallExpr->getSourceRange()), newCallExpr);
        }
      }
    }
}
  
void MutatorFrontendAction_443::MutatorASTConsumer_443::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    CallExprMatcher matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}