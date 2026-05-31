//header file
#pragma once
#include "Mutator_base.h"

/**
 * invalid_builtin_function_arguments_519
 */ 
class MutatorFrontendAction_519 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(519)

private:
    class MutatorASTConsumer_519 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_519(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/invalid_builtin_function_arguments_519.h"

// ========================================================================================================
#define MUT519_OUTPUT 1

void MutatorFrontendAction_519::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Call = Result.Nodes.getNodeAs<clang::CallExpr>("builtinCall")) {
      //Filter nodes in header files
      if (!Call || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Call->getBeginLoc()))
        return;

      if (Call->getNumArgs() > 0) {
        auto Arg = Call->getArg(0);
        auto ArgType = Arg->getType();

        // Perform mutation on the source code text by applying string replacement
        std::string replacement;
        if (ArgType->isPointerType()) {
          replacement = "0"; // Replace pointer with integer
        } else if (ArgType->isIntegerType()) {
          replacement = "nullptr"; // Replace integer with pointer
        } else {
          return; // Unsupported type for mutation
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Arg->getSourceRange()), replacement);
      }
    }
}
  
void MutatorFrontendAction_519::MutatorASTConsumer_519::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(callee(functionDecl(hasName("__builtin_assume_aligned")))).bind("builtinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}