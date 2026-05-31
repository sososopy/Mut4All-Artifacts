//header file
#pragma once
#include "Mutator_base.h"

/**
 * Swap_Function_Parameter_Types_498
 */ 

class MutatorFrontendAction_498 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(498)
private:
    class MutatorASTConsumer_498 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_498(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/swap_function_parameter_types_498.h"

// ========================================================================================================
#define MUT498_OUTPUT 1

void MutatorFrontendAction_498::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getNumParams() >= 2) {
        //Get the source code text of target node
        auto param1 = FD->getParamDecl(0);
        auto param2 = FD->getParamDecl(1);
        auto param1Type = param1->getType().getAsString();
        auto param2Type = param2->getType().getAsString();

        //Perform mutation on the source code text by applying string replacement
        std::string newFunctionDecl = FD->getReturnType().getAsString() + " " + FD->getNameAsString() + "(";
        newFunctionDecl += param2Type + " " + param1->getNameAsString() + ", ";
        newFunctionDecl += param1Type + " " + param2->getNameAsString();

        for (unsigned i = 2; i < FD->getNumParams(); ++i) {
          auto param = FD->getParamDecl(i);
          newFunctionDecl += ", " + param->getType().getAsString() + " " + param->getNameAsString();
        }
        newFunctionDecl += ")";

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), newFunctionDecl);
      }
    }
}
  
void MutatorFrontendAction_498::MutatorASTConsumer_498::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}