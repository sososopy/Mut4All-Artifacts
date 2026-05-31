//header file
#pragma once
#include "Mutator_base.h"

/**
 * Move_Local_Generic_Lambda_To_Global_433
 */ 
class MutatorFrontendAction_433 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(433)
    
    private:
      class MutatorASTConsumer_433 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_433(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<const clang::LambdaExpr *> cur_lambdas;
      };
    };

//source file
#include "../include/Move_Local_Generic_Lambda_To_Global_433.h"

// ========================================================================================================
#define MUT433_OUTPUT 1

void MutatorFrontendAction_433::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isGenericLambda() == false)
      return;
    llvm::outs() << "generic lambda found\n";
    cur_lambdas.push_back(DL);
  } else if (auto *FL = Result.Nodes.getNodeAs<clang::FunctionDecl>("Funcs")) {
    if (!FL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FL->getLocation()))
      return;
    if (!FL->hasBody())
      return;
    for (auto lambda : cur_lambdas) {
      if (FL->getSourceRange().fullyContains(lambda->getSourceRange())) {
        auto lambda_content = stringutils::rangetoStr(
            *(Result.SourceManager), lambda->getSourceRange());
        lambda_content =
            "/*mut433*/" + lambda_content + ";\n";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                lambda->getSourceRange()),
                            lambda->getLambdaClass()->getNameAsString());
        Rewrite.ReplaceText(FL->getBeginLoc(), 0, lambda_content);
      }
    }
  }
}

void MutatorFrontendAction_433::MutatorASTConsumer_433::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambdas");
    auto func_matcher = functionDecl().bind("Funcs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}