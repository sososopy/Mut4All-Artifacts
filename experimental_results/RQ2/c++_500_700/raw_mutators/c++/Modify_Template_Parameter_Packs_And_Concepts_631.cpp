//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_parameter_packs_and_concepts_631
 */ 
class MutatorFrontendAction_631 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(631)

private:
    class MutatorASTConsumer_631 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_631(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_parameter_packs_and_concepts_631.h"

// ========================================================================================================
#define MUT631_OUTPUT 1

void MutatorFrontendAction_631::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;
      
      auto lambdaRange = LambdaExpr->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);

      std::string mutatedLambdaText = lambdaText;
      size_t paramListStart = mutatedLambdaText.find('(');
      size_t paramListEnd = mutatedLambdaText.find(')', paramListStart);

      if (paramListStart != std::string::npos && paramListEnd != std::string::npos) {
          std::string paramList = mutatedLambdaText.substr(paramListStart + 1, paramListEnd - paramListStart - 1);
          mutatedLambdaText.insert(paramListEnd, ", " + paramList);
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), mutatedLambdaText);
    }
}
  
void MutatorFrontendAction_631::MutatorASTConsumer_631::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdaMatcher = lambdaExpr(hasAncestor(functionTemplateDecl())).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}