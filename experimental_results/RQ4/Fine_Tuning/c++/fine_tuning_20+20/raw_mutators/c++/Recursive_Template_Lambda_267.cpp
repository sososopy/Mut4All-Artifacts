//header file
#pragma once
#include "Mutator_base.h"

/**
 * Recursive_Template_Lambda_267
 */ 

class MutatorFrontendAction_267 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(267)
private:
    class MutatorASTConsumer_267 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_267(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/recursive_template_lambda_267.h"

// ========================================================================================================
#define MUT267_OUTPUT 1

void MutatorFrontendAction_267::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Templates")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isFunctionTemplateSpecialization() &&
          !FD->getDescribedFunctionTemplate())
        return;
      if (!FD->hasBody())
        return;
      auto body = FD->getBody();
      auto bodystr = stringutils::rangetoStr(*(Result.SourceManager),
                                             body->getSourceRange());
      llvm::outs() << bodystr << '\n';
      if (bodystr.find("return") != string::npos) {
        bodystr = bodystr.substr(0, bodystr.rfind("return")) +
                  "/*mut267*/return [&]{return " + FD->getNameAsString() +
                  "(0);}();\n";
      } else {
        bodystr = bodystr.substr(0, bodystr.rfind('}')) +
                  "/*mut267*/[&]{return " + FD->getNameAsString() +
                  "(0);}();\n}";
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()),
                          bodystr);
    }
}
  
void MutatorFrontendAction_267::MutatorASTConsumer_267::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}