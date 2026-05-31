//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_template_arguments_in_function_calls_10
 */ 
class MutatorFrontendAction_10 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(10)

private:
    class MutatorASTConsumer_10 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_10(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misuse_template_arguments_in_function_calls_10.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FC = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
      if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FC->getExprLoc()))
        return;

      const FunctionDecl *FD = FC->getDirectCallee();
      if (!FD || !FD->isFunctionTemplateSpecialization())
        return;

      const TemplateArgumentList *TAL = FC->getTemplateArgs();
      if (!TAL)
        return;

      std::string mutatedCall;
      if (TAL->size() > 0) {
        mutatedCall = FD->getNameInfo().getAsString() + "<int, double>(";
      } else {
        mutatedCall = FD->getNameInfo().getAsString() + "<int>(";
      }

      for (unsigned i = 0; i < FC->getNumArgs(); ++i) {
        if (i > 0)
          mutatedCall += ", ";
        mutatedCall += Lexer::getSourceText(CharSourceRange::getTokenRange(FC->getArg(i)->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
      }
      mutatedCall += ")";

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FC->getSourceRange()), mutatedCall);
    }
}
  
void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = callExpr(callee(functionDecl(isTemplateInstantiation()))).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}