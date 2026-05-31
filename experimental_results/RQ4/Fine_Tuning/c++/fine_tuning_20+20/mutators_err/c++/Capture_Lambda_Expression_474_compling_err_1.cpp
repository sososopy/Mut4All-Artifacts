//header file
#pragma once
#include "Mutator_base.h"

/**
 * Capture_Lambda_Expression_474
 */ 
class MutatorFrontendAction_474 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(474)

private:
    class MutatorASTConsumer_474 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_474(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/capture_lambda_expression_474.h"

// ========================================================================================================
#define MUT474_OUTPUT 1

void MutatorFrontendAction_474::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto lambda = stringutils::rangetoStr(*(Result.SourceManager),
                                            MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (lambda.find('[') != string::npos)
        lambda.replace(lambda.find('['), 1, "[=");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), lambda);
    }
}
  
void MutatorFrontendAction_474::MutatorASTConsumer_474::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}