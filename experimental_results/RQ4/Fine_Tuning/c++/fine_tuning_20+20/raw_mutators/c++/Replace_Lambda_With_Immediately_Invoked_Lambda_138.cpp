//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_With_Immediately_Invoked_Lambda_138
 */ 
class MutatorFrontendAction_138 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(138)

private:
    class MutatorASTConsumer_138 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_138(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_lambda_with_immediately_invoked_lambda_138.h"

// ========================================================================================================
#define MUT138_OUTPUT 1

void MutatorFrontendAction_138::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content;
      cur_lambdas.push_back(MT);
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CallExpr>("Calls")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getBeginLoc()))
        return;

      auto callee = CL->getCallee();
      if (callee == nullptr)
        return;
      auto type = callee->getType();
      if (type.isNull())
        return;
      if (!type->isFunctionPointerType())
        return;
      auto sub = callee->IgnoreCasts();
      if (sub == nullptr)
        return;
      if (sub->getStmtClass() != Stmt::StmtClass::DeclRefExprClass)
        return;
      auto DL = dyn_cast<DeclRefExpr>(sub);
      if (DL == nullptr)
        return;
      auto DLname = DL->getNameInfo().getAsString();
      llvm::outs() << DLname;
      for (size_t i = 0; i < cur_lambdas.size(); ++i) {
        auto lambda = cur_lambdas[i];
        auto lambda_name = "lambda_" + std::to_string(i);
        if (DLname == lambda_name) {
          auto lambda_content = stringutils::rangetoStr(
              *(Result.SourceManager), lambda->getSourceRange());
          auto call_content = stringutils::rangetoStr(
              *(Result.SourceManager), CL->getSourceRange());
          auto new_content = lambda_content + call_content.substr(DLname.size());
          new_content = "/*mut138*/" + new_content;
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()), new_content);
        }
      }
    }
}
  
void MutatorFrontendAction_138::MutatorASTConsumer_138::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambda_matcher = lambdaExpr().bind("Lambdas");
    auto call_matcher = callExpr().bind("Calls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.addMatcher(call_matcher, &callback);
    matchFinder.matchAST(Context);
}