//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Nested_Lambda_In_Variadic_Template_With_NonNested_Lambda_452
 */ 
class MutatorFrontendAction_452 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(452)

private:
    class MutatorASTConsumer_452 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_452(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::LambdaExpr *> cur_lambdas;
    };
};

//source file
#include "../include/replace_nested_lambda_in_variadic_template_with_nonnested_lambda_452.h"

// ========================================================================================================
#define MUT452_OUTPUT 1

void MutatorFrontendAction_452::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!LT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getBeginLoc()))
        return;
      if (!LT->hasExplicitParameters())
        return;
      if (LT->getCaptureDefault() != LambdaCaptureDefault::LCD_ByRef)
        return;
      if (LT->getCaptureKind() != LambdaCaptureKind::LCK_ByRef)
        return;
      if (LT->hasExplicitResultType())
        return;
      if (LT->getCaptureDefaultLoc().isValid())
        return;
      if (LT->getIntroducerRange().isValid())
        return;
      if (LT->isGenericLambda() == false)
        return;
      cur_lambdas.push_back(LT);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "VariadicFunctions")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (DL->isVariadic() == false)
        return;
      if (DL->hasBody() == false)
        return;
      if (cur_lambdas.size() == 0)
        return;
      auto body = DL->getBody();
      auto bodystr = stringutils::rangetoStr(*(Result.SourceManager),
                                             body->getSourceRange());
      auto lambda = cur_lambdas.back();
      auto lambdastr = stringutils::rangetoStr(*(Result.SourceManager),
                                               lambda->getSourceRange());
      llvm::outs() << lambdastr << '\n';
      if (lambdastr.find("[&](auto... p2s) { ([&] { p1s; p2s; }, ...); }") ==
          string::npos)
        return;
      string replacement = "[&](auto... p2s) { ([&] { p1s; }, ...); }";
      lambdastr.replace(lambdastr.find("[&](auto... p2s) { ([&] { p1s; p2s; }, "
                                        "...); }"),
                        string("[&](auto... p2s) { ([&] { p1s; p2s; }, ...); "
                               "}").length(),
                        replacement);
      lambdastr += "([&](auto... p2s) { p1s; p2s; }(args...));";
      llvm::outs() << lambdastr << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambda->getSourceRange()), lambdastr);
    }
}
  
void MutatorFrontendAction_452::MutatorASTConsumer_452::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambda_matcher = lambdaExpr().bind("Lambdas");
    auto variadic_matcher = functionDecl().bind("VariadicFunctions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.addMatcher(variadic_matcher, &callback);
    matchFinder.matchAST(Context);
}