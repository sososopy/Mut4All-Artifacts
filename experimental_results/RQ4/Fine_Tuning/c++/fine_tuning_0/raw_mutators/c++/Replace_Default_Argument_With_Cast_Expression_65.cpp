//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Argument_With_Cast_Expression_65
 */ 
class MutatorFrontendAction_65 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(65)

private:
    class MutatorASTConsumer_65 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_65(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Default_Argument_With_Cast_Expression_65.h"

// ========================================================================================================
#define MUT65_OUTPUT 1

void MutatorFrontendAction_65::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto parameters = MT->getTemplatedDecl()->parameters();
      for (auto param : parameters) {
        if (param->hasDefaultArg()) {
          auto default_arg = param->getDefaultArg();
          auto default_arg_text = stringutils::rangetoStr(
              *(Result.SourceManager), default_arg->getSourceRange());
          llvm::outs() << default_arg_text << '\n';
          auto param_type = param->getType();
          auto param_type_text = param_type.getAsString();
          llvm::outs() << param_type_text << '\n';
          //Perform mutation on the source code text by applying string replacement
          auto new_default_arg_text =
              "static_cast<" + param_type_text + ">(" + default_arg_text + ")";
          llvm::outs() << new_default_arg_text << '\n';
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(default_arg->getSourceRange(), new_default_arg_text);
        }
      }
    }
}
  
void MutatorFrontendAction_65::MutatorASTConsumer_65::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}