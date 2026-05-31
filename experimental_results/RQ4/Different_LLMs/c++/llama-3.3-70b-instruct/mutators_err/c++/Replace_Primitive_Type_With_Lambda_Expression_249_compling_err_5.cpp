//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Primitive_Type_With_Lambda_Expression_249
 */ 
class MutatorFrontendAction_249 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(249)

private:
    class MutatorASTConsumer_249 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_249(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Primitive_Type_With_Lambda_Expression_249.h"
#include "clang/AST/Type.h"
#include "clang/AST/ASTContext.h"

// ========================================================================================================
#define MUT249_OUTPUT 1

void MutatorFrontendAction_249::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto params = FD->parameters();
      for (auto param : params) {
        //Perform mutation on the source code text by applying string replacement
        if (param->getType()->isIntegerType() || param->getType()->isFloatingType()) {
          std::string paramType = param->getType()->getAsString(*Result.Context);
          std::string lambdaExpr = "[x](){}";
          std::string mutatedParamType = lambdaExpr;
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(param->getSourceRange(), mutatedParamType);
        }
      }
      //Perform mutation on return type
      if (FD->getReturnType()->isIntegerType() || FD->getReturnType()->isFloatingType()) {
        std::string returnType = FD->getReturnType()->getAsString(*Result.Context);
        std::string lambdaExpr = "[x](){}";
        std::string mutatedReturnType = lambdaExpr;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(FD->getReturnTypeSourceRange(), mutatedReturnType);
      }
    }
}

void MutatorFrontendAction_249::MutatorASTConsumer_249::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}