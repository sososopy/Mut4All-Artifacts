//header file
#pragma once
#include "Mutator_base.h"

/**
 * Swap_Parameter_With_Function_Call_278
 */ 
class MutatorFrontendAction_278 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(278)

private:
    class MutatorASTConsumer_278 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_278(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> cur_functions;
    };
};

//source file
#include "../include/Swap_Parameter_With_Function_Call_278.h"

// ========================================================================================================
#define MUT278_OUTPUT 1

void MutatorFrontendAction_278::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      cur_functions.push_back(FD);
      llvm::outs() << "Function Name: " << FD->getNameAsString() << "\n";
    } else if (auto *PD = Result.Nodes.getNodeAs<clang::ParmVarDecl>(
                   "Parameters")) {
      if (!PD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PD->getLocation()))
        return;
      auto PT = PD->getType();
      auto PL = PD->getLocation();
      llvm::outs() << "Parameter Type: " << PT.getAsString() << "\n";
      llvm::outs() << "Parameter Loc: " << PL.printToString(Rewrite.getSourceMgr()) << "\n";
      auto parent = PD->getParentFunctionOrMethod();
      if (parent == nullptr)
        return;
      llvm::outs() << "Parent Function: " << parent->getNameAsString() << "\n";
      std::vector<const clang::FunctionDecl *> candidate_functions;
      for (std::size_t j = 0; j < cur_functions.size(); ++j) {
        if (cur_functions[j]->getReturnType() == PT &&
            cur_functions[j] != parent)
          candidate_functions.push_back(cur_functions[j]);
      }
      if (candidate_functions.empty()) {
        llvm::outs() << "No candidate function found!\n";
        return;
      }
      size_t index = getrandom::getRandomIndex(candidate_functions.size() - 1);
      auto target_function = candidate_functions[index];
      auto target_function_name = target_function->getNameAsString();
      llvm::outs() << "Target Function: " << target_function_name << "\n";
      Rewrite.ReplaceText(PL, target_function_name);
    }
  }

void MutatorFrontendAction_278::MutatorASTConsumer_278::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto parameter_matcher = parmVarDecl().bind("Parameters");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(parameter_matcher, &callback);
    matchFinder.matchAST(Context);
}