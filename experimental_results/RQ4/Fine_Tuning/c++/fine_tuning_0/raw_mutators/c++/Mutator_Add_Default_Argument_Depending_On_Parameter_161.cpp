//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Add_Default_Argument_Depending_On_Parameter_161
 */ 
class MutatorFrontendAction_161 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(161)

private:
    class MutatorASTConsumer_161 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_161(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_Add_Default_Argument_Depending_On_Parameter_161.h"

// ========================================================================================================
#define MUT161_OUTPUT 1

void MutatorFrontendAction_161::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getNumParams() == 0)
        return;
      //Record the candidate existing function names for insertion
      cur_functions.push_back(FD);
    } else if (auto *PD = Result.Nodes.getNodeAs<clang::ParmVarDecl>("Params")) {
      if (!PD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PD->getLocation()))
        return;
      if (PD->hasDefaultArg() == false)
        return;
      //Get the source code text of target node
      auto default_expr = PD->getDefaultArg();
      auto default_expr_str = stringutils::rangetoStr(
          *(Result.SourceManager), default_expr->getSourceRange());
      llvm::outs() << default_expr_str << '\n';
      //Perform mutation on the source code text by applying string replacement
      for (std::size_t j = 0; j < cur_functions.size(); ++j) {
        auto params = cur_functions[j]->parameters();
        for (std::size_t i = 0; i < params.size(); ++i) {
          if (params[i] == PD) {
            if (default_expr->getStmtClass() ==
                Stmt::StmtClass::IntegerLiteralClass) {
              if (i == 0)
                continue;
              auto new_default_expr_str = params[i - 1]->getNameAsString();
              new_default_expr_str =
                  "/*mut161*/" + new_default_expr_str + '(' +
                  new_default_expr_str + ')';
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(
                  CharSourceRange::getTokenRange(default_expr->getSourceRange()),
                  new_default_expr_str);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_161::MutatorASTConsumer_161::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    auto matcher = functionDecl().bind("Functions");
    auto param_matcher = parmVarDecl().bind("Params");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(param_matcher, &callback);
    matchFinder.matchAST(Context);
}