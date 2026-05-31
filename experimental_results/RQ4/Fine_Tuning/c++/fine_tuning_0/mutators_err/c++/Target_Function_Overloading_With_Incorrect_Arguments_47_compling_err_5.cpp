//header file
#pragma once
#include "Mutator_base.h"

/**
 * Target_Function_Overloading_With_Incorrect_Arguments_47
 */ 
class MutatorFrontendAction_47 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(47)

private:
    class MutatorASTConsumer_47 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_47(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> cur_functions;
        std::vector<std::string> cur_function_names;
        std::vector<int> cur_function_argnum;
    };
};

//source file
#include "../include/Target_Function_Overloading_With_Incorrect_Arguments_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto name = FD->getNameAsString();
      if (FD->isOverloadedOperator() || name == "")
        return;
      auto argnum = FD->getNumParams();
      cur_functions.push_back(FD);
      cur_function_names.push_back(name);
      cur_function_argnum.push_back(argnum);
    } else if (auto *CE =
                   Result.Nodes.getNodeAs<clang::CallExpr>("CallExprs")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      auto callee = CE->getCalleeDecl();
      if (callee == nullptr)
        return;
      if (callee->getKind() != Decl::Kind::Function)
        return;
      auto callee_func = dyn_cast<FunctionDecl>(callee);
      llvm::outs() << callee_func->getNameAsString() << '\n';
      auto callee_name = callee_func->getNameAsString();
      auto argnum = callee_func->getNumParams();
      std::vector<const clang::FunctionDecl *> candidates;
      for (std::size_t i = 0; i < cur_functions.size(); ++i) {
        if (cur_function_names[i] == callee_name &&
            cur_function_argnum[i] == argnum) {
          candidates.push_back(cur_functions[i]);
        }
      }
      if (candidates.size() <= 1)
        return;
      auto choice = getrandom::getRandomIndex(candidates.size() - 1);
      auto target = candidates[choice];
      llvm::outs() << target->getNameAsString() << '\n';
      auto target_params = target->parameters();
      std::vector<QualType> target_types;
      for (auto param : target_params) {
        target_types.push_back(param->getType());
      }
      auto args = CE->getArgs();
      std::vector<std::string> arg_strs;
      for (auto arg : args) {
        auto arg_str = stringutils::rangetoStr(*(Result.SourceManager),
                                               arg->getSourceRange());
        arg_strs.push_back(arg_str);
      }
      std::vector<std::string> new_args;
      for (std::size_t i = 0; i < arg_strs.size(); ++i) {
        if (args[i]->getType() != target_types[i]) {
          new_args.push_back(arg_strs[i]);
        }
      }
      if (new_args.size() != arg_strs.size())
        return;
      std::string new_call = callee_name + "(";
      for (std::size_t i = 0; i < new_args.size(); ++i) {
        new_call += new_args[i];
        if (i != new_args.size() - 1)
          new_call += ",";
      }
      new_call += ")";
      llvm::outs() << new_call << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()),
                          "/*mut47*/" + new_call);
    }
}
  
void MutatorFrontendAction_47::MutatorASTConsumer_47::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto function_matcher = functionDecl().bind("Functions");
    auto callexpr_matcher = callExpr().bind("CallExprs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.addMatcher(callexpr_matcher, &callback);
    matchFinder.matchAST(Context);
}