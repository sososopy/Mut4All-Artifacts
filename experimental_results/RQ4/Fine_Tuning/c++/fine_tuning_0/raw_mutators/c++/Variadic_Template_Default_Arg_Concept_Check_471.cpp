//header file
#pragma once
#include "Mutator_base.h"

/**
 * Variadic_Template_Default_Arg_Concept_Check_471
 */ 
class MutatorFrontendAction_471 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(471)

private:
    class MutatorASTConsumer_471 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_471(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Variadic_Template_Default_Arg_Concept_Check_471.h"

// ========================================================================================================
#define MUT471_OUTPUT 1

void MutatorFrontendAction_471::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplates")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << declaration << '\n';
      auto params = MT->getTemplateParameters();
      bool has_variadic = false;
      for (auto param : *params) {
        if (param->isTemplateParameterPack()) {
          has_variadic = true;
          break;
        }
      }
      if (!has_variadic)
        return;
      auto func = MT->getTemplatedDecl();
      auto func_params = func->parameters();
      if (func_params.size() == 0)
        return;
      auto first_param = func_params[0];
      if (!first_param->hasDefaultArg())
        return;
      auto default_arg = first_param->getDefaultArg();
      auto default_arg_str = stringutils::rangetoStr(
          *(Result.SourceManager), default_arg->getSourceRange());
      llvm::outs() << default_arg_str << '\n';
      string new_default_arg = "(std::integral<U> &&...)";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(default_arg->getSourceRange(), new_default_arg);
    }
}
  
void MutatorFrontendAction_471::MutatorASTConsumer_471::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FuncTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}