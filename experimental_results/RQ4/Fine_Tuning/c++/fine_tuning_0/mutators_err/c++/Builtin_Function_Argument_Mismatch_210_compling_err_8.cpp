//header file
#pragma once
#include "Mutator_base.h"

/**
 * Builtin_Function_Argument_Mismatch_210
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
#include "../include/Builtin_Function_Argument_Mismatch_210.h"

// ========================================================================================================
#define MUT210_OUTPUT 1

void MutatorFrontendAction_210::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("BuiltinCalls")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto callee = MT->getDirectCallee();
      if (!callee || !callee->getBuiltinID())
        return;
      auto args = MT->getArgs();
      if (args.empty())
        return;
      auto first_arg = args[0];
      auto first_arg_type = first_arg->getType();
      if (first_arg_type->isPointerType()) {
        //Perform mutation on the source code text by applying string replacement
        auto replacement = "42";
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(first_arg->getSourceRange(), replacement);
      }
    }
}
  
void MutatorFrontendAction_210::MutatorASTConsumer_210::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr().bind("BuiltinCalls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}