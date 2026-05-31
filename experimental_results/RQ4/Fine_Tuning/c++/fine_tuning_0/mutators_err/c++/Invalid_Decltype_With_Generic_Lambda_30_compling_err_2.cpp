//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Decltype_With_Generic_Lambda_30
 */ 
class MutatorFrontendAction_30 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(30)

private:
    class MutatorASTConsumer_30 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_30(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalid_Decltype_With_Generic_Lambda_30.h"

// ========================================================================================================
#define MUT30_OUTPUT 1

void MutatorFrontendAction_30::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("generic_lambda")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (DL->getExplicitTemplateParameterList() == nullptr)
        return;
      if (DL->isGenericLambda() == false)
        return;
      //Get the source code text of target node
      auto lambda_name = DL->getLambdaClass()->getNameAsString();
      //Perform mutation on the source code text by applying string replacement
      auto decltype_decl = "decltype(" + lambda_name + "()) mut_30;";
      llvm::outs() << decltype_decl << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(DL->getEndLoc(), 0, "/*mut30*/" + decltype_decl);
    }
}
  
void MutatorFrontendAction_30::MutatorASTConsumer_30::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("generic_lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}