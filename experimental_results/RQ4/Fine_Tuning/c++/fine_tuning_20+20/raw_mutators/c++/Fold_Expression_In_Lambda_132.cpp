//header file
#pragma once
#include "Mutator_base.h"

/**
 * Fold_Expression_In_Lambda_132
 */ 
class MutatorFrontendAction_132 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(132)

private:
    class MutatorASTConsumer_132 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_132(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> cur_funcs;
    };
};

//source file
#include "../include/fold_expression_in_lambda_132.h"

// ========================================================================================================
#define MUT132_OUTPUT 1

void MutatorFrontendAction_132::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isTemplated() || FD->isDefaulted())
        return;
      if (FD->hasBody() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      if (content.find("[]") == string::npos)
        return;
      if (content.find("[]") != content.rfind("[]"))
        return;
      if (content.find("[]") != content.rfind("[]") ||
          content.find("[]") == string::npos)
        return;
      if (content.find("[]") == content.rfind("[]")) {
        content.insert(content.find("[]"), "<int... Is>");
        content.insert(content.find("[]") + 2, "int = 0");
        content.insert(content.rfind("}"), "lambda1.template operator()<1,2,3>();");
        content.insert(content.rfind("}"), "([a = Is]{}, ...);");
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), content);
    }
  }

void MutatorFrontendAction_132::MutatorASTConsumer_132::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}