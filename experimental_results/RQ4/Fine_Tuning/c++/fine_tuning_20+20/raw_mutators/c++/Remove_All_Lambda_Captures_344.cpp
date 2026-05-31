//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_All_Lambda_Captures_344
 */ 
class MutatorFrontendAction_344 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(344)

private:
    class MutatorASTConsumer_344 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_344(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/remove_all_lambda_captures_344.h"

// ========================================================================================================
#define MUT344_OUTPUT 1

void MutatorFrontendAction_344::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto lambda = stringutils::rangetoStr(*(Result.SourceManager),
                                            MT->getSourceRange());
      llvm::outs() << lambda << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (lambda.find('[') != string::npos &&
          lambda.find(']') != string::npos) {
        auto capture = lambda.substr(lambda.find('[') + 1,
                                     lambda.find(']') - lambda.find('[') - 1);
        llvm::outs() << capture << '\n';
        if (capture != "") {
          lambda.replace(lambda.find('[') + 1, capture.size(), "&");
          llvm::outs() << lambda << '\n';
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), lambda);
    }
}
  
void MutatorFrontendAction_344::MutatorASTConsumer_344::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}