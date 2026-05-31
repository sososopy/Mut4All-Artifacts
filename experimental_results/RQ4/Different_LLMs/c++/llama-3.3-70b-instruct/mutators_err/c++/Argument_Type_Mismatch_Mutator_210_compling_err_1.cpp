```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Argument_Type_Mismatch_Mutator_210
 */ 
class MutatorFrontendAction_210 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(210)

private:
    class MutatorASTConsumer_210 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_210(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Argument_Type_Mismatch_Mutator_210.h"

// ========================================================================================================
#define MUT210_OUTPUT 1

void MutatorFrontendAction_210::Callback::run(const MatchFinder::MatchResult &Result) {
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
      auto arg = args[0];
      auto argType = arg->getType();
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedArg;
      if (argType->isIntegerType()) {
        mutatedArg = "nullptr"; // Replace integer with a pointer
      } else if (argType->isPointerType()) {
        mutatedArg = "10"; // Replace pointer with an integer
      } else {
        return;
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(arg->getSourceRange(), mutatedArg);
    }
}
  
void MutatorFrontendAction_210::MutatorASTConsumer_210::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    CallExprMatcher matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}