//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Constraint_From_Generic_Lambda_Return_90
 */ 
class MutatorFrontendAction_90 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(90)

private:
    class MutatorASTConsumer_90 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_90(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/mutator_90.h"

// ========================================================================================================
#define MUT90_OUTPUT 1

void MutatorFrontendAction_90::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (content.find("requires") != string::npos) {
        content = content.substr(0, content.find("requires")) +
                  content.substr(content.find(")") + 1);
        llvm::outs() << content << '\n';
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(DL->getSourceRange()), content);
      }
    }
}

void MutatorFrontendAction_90::MutatorASTConsumer_90::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = functionDecl(returns(lambdaType(hasLambdaParameter(
                               hasType(qualType(hasConstraint()))))))
                       .bind("Functions");
    auto lambda_matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.matchAST(Context);
}