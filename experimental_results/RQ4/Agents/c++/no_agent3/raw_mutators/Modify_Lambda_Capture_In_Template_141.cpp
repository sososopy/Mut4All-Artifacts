//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Capture_In_Template_141
 */ 
class MutatorFrontendAction_141 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(141)

private:
    class MutatorASTConsumer_141 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_141(Rewriter &R) : TheRewriter(R) {}
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
        const CXXMethodDecl *currentTemplateFunction = nullptr;
    };
};

//source file
#include "../include/modify_lambda_capture_in_template_141.h"

// ========================================================================================================
#define MUT141_OUTPUT 1

void MutatorFrontendAction_141::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *lambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!lambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     lambdaExpr->getBeginLoc()))
        return;

      if (currentTemplateFunction) {
        // Get the source code text of target node
        auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                    lambdaExpr->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string additionalCapture = "&extra";
        std::size_t pos = lambdaSource.find("](");
        if (pos != std::string::npos) {
            lambdaSource.insert(pos, ", " + additionalCapture);
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaExpr->getSourceRange()), lambdaSource);
      }
    } else if (auto *templateFunc = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("templateFunc")) {
      // Record the node information to be used in the mutation process
      currentTemplateFunction = templateFunc->getTemplatedDecl();
    }
}
  
void MutatorFrontendAction_141::MutatorASTConsumer_141::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    auto lambdaMatcher = lambdaExpr(hasAncestor(functionTemplateDecl().bind("templateFunc"))).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}