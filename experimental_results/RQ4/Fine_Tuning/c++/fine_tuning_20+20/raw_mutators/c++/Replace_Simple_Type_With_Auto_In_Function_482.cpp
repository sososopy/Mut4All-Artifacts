//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Simple_Type_With_Auto_In_Function_482
 */ 
class MutatorFrontendAction_482 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(482)

private:
    class MutatorASTConsumer_482 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_482(Rewriter &R) : TheRewriter(R) {}
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
        vector<const clang::FunctionDecl *> decls;
        vector<const clang::CallExpr *> calls;
    };
};

//source file
#include "../include/replace_simple_type_with_auto_in_function_482.h"

// ========================================================================================================
#define MUT482_OUTPUT 1

void MutatorFrontendAction_482::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getBeginLoc()))
      return;
    // if (!FD->isThisDeclarationADefinition())
    //   return;
    if (FD->isTemplateInstantiation())
      return;
    if (FD->isDependentContext())
      return;
    decls.push_back(FD);
    return;
  } else if (auto *CE =
                 Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
    if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CE->getBeginLoc()))
      return;
    calls.push_back(CE);
    return;
  }
  int choice = getrandom::getRandomIndex(1);
  if (choice == 0) {
    if (decls.empty())
      return;
    size_t index = getrandom::getRandomIndex(decls.size() - 1);
    auto target = decls[index];
    if (!target->isThisDeclarationADefinition())
      return;
    auto params = target->parameters();
    if (params.empty())
      return;
    vector<int> simple_index;
    for (size_t i = 0; i < params.size(); ++i) {
      if (params[i]->getType()->isBuiltinType())
        simple_index.push_back(i);
    }
    if (simple_index.empty())
      return;
    size_t index2 = getrandom::getRandomIndex(simple_index.size() - 1);
    auto target_param = params[simple_index[index2]];
    Rewrite.ReplaceText(target_param->getBeginLoc(), 0, "/*mut482*/auto");
    Rewrite.ReplaceText(target_param->getBeginLoc(), 3, "");
  } else {
    if (calls.empty())
      return;
    size_t index = getrandom::getRandomIndex(calls.size() - 1);
    auto target = calls[index];
    auto callee = target->getDirectCallee();
    if (callee == nullptr)
      return;
    if (callee->isTemplateInstantiation())
      return;
    if (callee->isDependentContext())
      return;
    auto args = target->getArgs();
    if (args == nullptr)
      return;
    vector<int> simple_index;
    for (size_t i = 0; i < args.size(); ++i) {
      if (args[i]->getType()->isBuiltinType())
        simple_index.push_back(i);
    }
    if (simple_index.empty())
      return;
    size_t index2 = getrandom::getRandomIndex(simple_index.size() - 1);
    auto target_arg = args[simple_index[index2]];
    Rewrite.ReplaceText(target_arg->getBeginLoc(), 0, "/*mut482*/auto");
    Rewrite.ReplaceText(target_arg->getBeginLoc(), 3, "");
    auto params = callee->parameters();
    auto target_param = params[simple_index[index2]];
    Rewrite.ReplaceText(target_param->getBeginLoc(), 0, "/*mut482*/auto");
    Rewrite.ReplaceText(target_param->getBeginLoc(), 3, "");
  }
}
  
void MutatorFrontendAction_482::MutatorASTConsumer_482::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto decl_matcher = functionDecl().bind("FunctionDecl");
    auto call_matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.addMatcher(call_matcher, &callback);
    matchFinder.matchAST(Context);
}