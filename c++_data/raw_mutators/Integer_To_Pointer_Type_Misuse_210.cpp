//header file
#pragma once
#include "Mutator_base.h"

/**
 * Integer_To_Pointer_Type_Misuse_210
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
    };
};

//source file
#include "../include/Mutator_210.h"

// ========================================================================================================
#define MUT210_OUTPUT 1

void MutatorFrontendAction_210::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
      //Filter nodes in header files
      if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CallExpr->getExprLoc()))
        return;

      for (unsigned i = 0; i < CallExpr->getNumArgs(); ++i) {
        auto Arg = CallExpr->getArg(i);
        if (Arg->getType()->isPointerType()) {
          //Perform mutation on the source code text by applying string replacement
          std::string replacement = "42"; // Example integer literal
          Rewrite.ReplaceText(Arg->getSourceRange(), replacement);
          break; // Apply mutation to the first pointer argument found
        }
      }
    }
}
  
void MutatorFrontendAction_210::MutatorASTConsumer_210::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(hasAnyArgument(hasType(pointerType()))).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}