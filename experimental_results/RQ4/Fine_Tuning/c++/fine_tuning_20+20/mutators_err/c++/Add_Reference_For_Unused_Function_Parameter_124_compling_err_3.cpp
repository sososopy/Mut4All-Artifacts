//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Reference_For_Unused_Function_Parameter_124
 */ 
class MutatorFrontendAction_124 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(124)
    
    private:
      class MutatorASTConsumer_124 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_124(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<const clang::FunctionDecl *> candidate_funcs;
      };
    };

//source file
#include "../include/add_reference_for_unused_function_parameter_124"

// ========================================================================================================
#define MUT124_OUTPUT 1

void MutatorFrontendAction_124::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    if (!FD->isTemplateInstantiation())
      return;
    candidate_funcs.push_back(FD);
  } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Target")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    if (!FD->hasBody())
      return;
    auto params = FD->parameters();
    std::vector<std::size_t> unused_params;
    for (std::size_t i = 0; i < params.size(); ++i) {
      auto param = params[i];
      auto ptype = param->getType();
      if (ptype->isDependentType() == false)
        continue;
      if (param->isUsed())
        continue;
      unused_params.push_back(i);
    }
    if (unused_params.empty())
      return;
    std::size_t choice = getrandom::getRandomIndex(unused_params.size() - 1);
    auto target_param = params[unused_params[choice]];
    auto target_name = target_param->getNameAsString();
    auto body = FD->getBody();
    auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                            body->getSourceRange());
    if (FD->getReturnType()->isDependentType())
      body_str.insert(body_str.rfind('}'), "\nreturn " + target_name + ";\n");
    else
      body_str.insert(body_str.rfind('}'), "\n/*mut124*/" + target_name + ";\n");
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()),
                        body_str);
  }
}

void MutatorFrontendAction_124::MutatorASTConsumer_124::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto target_matcher = functionDecl().bind("Target");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(target_matcher, &callback);
    matchFinder.matchAST(Context);
}