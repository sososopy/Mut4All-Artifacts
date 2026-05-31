//header file
#pragma once
#include "Mutator_base.h"

/**
 * Apply_Pseudo_Destructor_In_Function_Definition_211
 */ 
class MutatorFrontendAction_211 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(211)

private:
    class MutatorASTConsumer_211 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_211(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> cur_vars;
        std::vector<const clang::Expr *> cur_exprs;
    };
};

//source file
#include "../include/apply_pseudo_destructor_in_function_definition_211.h"

// ========================================================================================================
#define MUT211_OUTPUT 1

void MutatorFrontendAction_211::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isThisDeclarationADefinition())
        return;
      auto body = FD->getBody();
      auto vars = FD->parameters();
      for (auto var : vars) {
        cur_vars.push_back(var);
      }
      auto exprs = body->children();
      for (auto expr : exprs) {
        cur_exprs.push_back(expr);
      }
    } else if (auto *LB = Result.Nodes.getNodeAs<clang::LambdaExpr>(
                   "LambdaExpressions")) {
      if (!LB || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LB->getBeginLoc()))
        return;
      auto body = LB->getBody();
      auto vars = LB->capture_inits();
      for (auto var : vars) {
        cur_exprs.push_back(var);
      }
      auto exprs = body->children();
      for (auto expr : exprs) {
        cur_exprs.push_back(expr);
      }
    } else if (auto *EX = Result.Nodes.getNodeAs<clang::Expr>("Exprs")) {
      if (!EX || !Result.Context->getSourceManager().isWrittenInMainFile(
                     EX->getBeginLoc()))
        return;
      if (cur_vars.empty() && cur_exprs.empty())
        return;
      std::size_t type_choice = 0;
      std::size_t expr_choice = 0;
      if (!cur_vars.empty())
        type_choice = getrandom::getRandomIndex(1);
      if (type_choice == 0) {
        expr_choice = getrandom::getRandomIndex(cur_vars.size() - 1);
        auto target = cur_vars[expr_choice];
        auto type = target->getType().getAsString();
        auto name = target->getNameAsString();
        auto ins = "(" + name + ").~" + type + "()";
        llvm::outs() << ins << '\n';
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(EX->getSourceRange()), ins);
      } else {
        expr_choice = getrandom::getRandomIndex(cur_exprs.size() - 1);
        auto target = cur_exprs[expr_choice];
        auto type = target->getType().getAsString();
        auto content =
            stringutils::rangetoStr(*(Result.SourceManager), target->getSourceRange());
        auto ins = "(" + content + ").~" + type + "()";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(EX->getSourceRange()), ins);
      }
    }
}
  
void MutatorFrontendAction_211::MutatorASTConsumer_211::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto lambda_matcher = lambdaExpr().bind("LambdaExpressions");
    auto expr_matcher = expr().bind("Exprs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.addMatcher(expr_matcher, &callback);
    matchFinder.matchAST(Context);
}