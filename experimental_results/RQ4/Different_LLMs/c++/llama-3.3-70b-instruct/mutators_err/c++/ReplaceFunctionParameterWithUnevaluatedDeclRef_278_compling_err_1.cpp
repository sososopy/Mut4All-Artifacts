```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceFunctionParameterWithUnevaluatedDeclRef_278
 */ 
class MutatorFrontendAction_278 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(278)

private:
    class MutatorASTConsumer_278 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_278(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ReplaceFunctionParameterWithUnevaluatedDeclRef_278.h"

// ========================================================================================================
#define MUT278_OUTPUT 1

void MutatorFrontendAction_278::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Call = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!Call || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Call->getLocStart()))
        return;
      //Get the source code text of target node
      auto args = Call->args();
      if (args.size() == 0)
        return;
      //Perform mutation on the source code text by applying string replacement
      for (auto arg : args) {
        if (auto *Var = dyn_cast<clang::DeclRefExpr>(arg)) {
          if (auto *VarDecl = Var->getDecl()) {
            if (!VarDecl->isUsedOutsideOfLambdas()) {
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(arg->getSourceRange(), "&" + VarDecl->getNameAsString());
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_278::MutatorASTConsumer_278::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    CallExprMatcher matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}