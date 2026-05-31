//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Template_Parameters_In_Lambda_Expressions_479
 */ 
class MutatorFrontendAction_479 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(479)

private:
    class MutatorASTConsumer_479 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_479(Rewriter &R) : TheRewriter(R) {}
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
        std::string newConceptName = "D";
    };
};

//source file
#include "../include/Alter_Template_Parameters_In_Lambda_Expressions_479.h"

// ========================================================================================================
#define MUT479_OUTPUT 1

void MutatorFrontendAction_479::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      auto &Context = *Result.Context;
      auto &SM = Context.getSourceManager();
      auto &LangOpts = Context.getLangOpts();
      auto &Rewrite = this->Rewrite;

      auto LambdaRange = LambdaExpr->getSourceRange();
      auto LambdaText = Lexer::getSourceText(CharSourceRange::getTokenRange(LambdaRange), SM, LangOpts).str();

      std::string newLambdaText = LambdaText;
      size_t templatePos = newLambdaText.find("<");
      if (templatePos != std::string::npos) {
          newLambdaText.insert(templatePos + 1, newConceptName + ", ");
      }

      size_t operatorPos = newLambdaText.find("operator()<");
      if (operatorPos != std::string::npos) {
          newLambdaText.insert(operatorPos + 11, "char, ");
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaRange), newLambdaText);
    }
}
  
void MutatorFrontendAction_479::MutatorASTConsumer_479::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(hasTemplateParameterList()).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}