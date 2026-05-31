//header file
#pragma once
#include "Mutator_base.h"

/**
 * LambdaCapturePackExpansion_460
 */ 
class MutatorFrontendAction_460 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(460)

private:
    class MutatorASTConsumer_460 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_460(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/LambdaCapturePackExpansion_460.h"

// ========================================================================================================
#define MUT460_OUTPUT 1

void MutatorFrontendAction_460::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!Lambda || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Lambda->getLocation()))
        return;
      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), Lambda->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = lambdaText;
      // Find the parameter pack expansion in the lambda expression
      auto paramPackExpansion = Lambda->getCaptureAs<clang::PackExpansionExpr>();
      if (paramPackExpansion) {
        // Replace the fold expression with a comma-separated list of captured parameters
        std::string capturedParams;
        for (auto param : Lambda->getCaptureAs<clang::PackExpansionExpr>()->getPattern()->children()) {
          capturedParams += param->getNameAsString() + ", ";
        }
        capturedParams.pop_back(); // Remove the trailing comma and space
        capturedParams.pop_back();
        mutatedText.replace(mutatedText.find("..."), 3, capturedParams);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Lambda->getSourceRange()), mutatedText);
    }
}

void MutatorFrontendAction_460::MutatorASTConsumer_460::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(hasAnyCapture(hasType(packExpansionType()))).bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}