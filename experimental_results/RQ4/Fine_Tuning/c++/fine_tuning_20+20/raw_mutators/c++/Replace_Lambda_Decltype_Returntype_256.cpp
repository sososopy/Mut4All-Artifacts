//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Decltype_Returntype_256
 */ 
class MutatorFrontendAction_256 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(256)

private:
    class MutatorASTConsumer_256 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_256(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_lambda_decltype_returntype_256.h"

// ========================================================================================================
#define MUT256_OUTPUT 1

void MutatorFrontendAction_256::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LD = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      //Filter nodes in header files
      if (!LD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LD->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambda = stringutils::rangetoStr(*(Result.SourceManager),
                                            LD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (lambda.find("decltype") != string::npos) {
        if (lambda.find("decltype") != string::npos) {
          lambda.replace(lambda.find("decltype"), 8, "void");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(LD->getSourceRange()), lambda);
    }
}
  
void MutatorFrontendAction_256::MutatorASTConsumer_256::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher =
        functionDecl(hasBody(compoundStmt(has(lambdaExpr())))).bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}