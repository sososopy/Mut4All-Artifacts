//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Body_With_Static_Assert_286
 */ 
class MutatorFrontendAction_286 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(286)

private:
    class MutatorASTConsumer_286 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_286(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Lambda_Body_With_Static_Assert_286.h"

// ========================================================================================================
#define MUT286_OUTPUT 1

void MutatorFrontendAction_286::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      llvm::outs() << "lambda expr found\n";
      auto lambda_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                 LE->getSourceRange());
      llvm::outs() << lambda_text << '\n';
      auto lambda_capture = LE->capture_begin();
      auto capture_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                  lambda_capture->getLocation());
      llvm::outs() << capture_text << '\n';
      auto lambda_body = LE->getBody();
      auto body_text = stringutils::rangetoStr(*(Result.SourceManager),
                                               lambda_body->getSourceRange());
      llvm::outs() << body_text << '\n';
      string new_body =
          "{using Callable = decltype(" + capture_text +
          ");\nstatic_assert(std::is_same_v<decltype(std::declval<const Callable>()."
          "operator()(std::declval<decltype(args)>()...)"
          "), bool>, \"test failed\");\n return " +
          capture_text + "(args...);}";
      lambda_text = stringutils::replaceStr(lambda_text, body_text, new_body);
      llvm::outs() << lambda_text << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()),
                          lambda_text);
    }
}
  
void MutatorFrontendAction_286::MutatorASTConsumer_286::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(has(cxxConstructExpr())).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}