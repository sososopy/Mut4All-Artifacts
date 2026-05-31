//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_decltype_in_lambda_564
 */ 
class MutatorFrontendAction_564 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(564)

private:
    class MutatorASTConsumer_564 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_564(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_decltype_in_lambda_564.h"

// ========================================================================================================
#define MUT564_OUTPUT 1

void MutatorFrontendAction_564::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getLocation()))
        return;

      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), LE->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t decltypePos = lambdaText.find("decltype(");
      if (decltypePos != std::string::npos) {
          size_t endPos = lambdaText.find(')', decltypePos);
          if (endPos != std::string::npos) {
              std::string originalDecltype = lambdaText.substr(decltypePos, endPos - decltypePos + 1);
              std::string mutatedDecltype = "decltype(5 + " + lambdaText.substr(decltypePos + 9, endPos - decltypePos - 9) + ")";
              lambdaText.replace(decltypePos, endPos - decltypePos + 1, mutatedDecltype);
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambdaText);
    }
}
  
void MutatorFrontendAction_564::MutatorASTConsumer_564::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasDescendant(decltypeType())).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}