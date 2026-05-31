//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Argument_Mismatch_In_Variadic_Function_Template_Call_196
 */ 
class MutatorFrontendAction_196 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(196)

private:
    class MutatorASTConsumer_196 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_196(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl *> variadic_function_templates;
    };
};

//source file
#include "../include/Template_Argument_Mismatch_In_Variadic_Function_Template_Call_196.h"

// ========================================================================================================
#define MUT196_OUTPUT 1

void MutatorFrontendAction_196::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("VariadicFunctionTemplate")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FT->getLocation()))
          return;
        if (FT->getTemplateParameters()->size() != 1)
          return;
        auto param = FT->getTemplateParameters()->getParam(0);
        if (param->isTemplateParameterPack()) {
          variadic_function_templates.push_back(FT);
        }
      } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CE->getBeginLoc()))
          return;
        if (CE->getNumArgs() == 0)
          return;
        if (CE->getNumArgs() != CE->getNumTemplateArgs())
          return;
        auto callee = CE->getCalleeDecl();
        if (!callee)
          return;
        if (auto callee_template = callee->getDescribedTemplate()) {
          for (auto variadic_function_template : variadic_function_templates) {
            if (variadic_function_template == callee_template) {
              auto template_args = CE->template_arguments();
              auto template_args_str = stringutils::rangetoStr(
                  *(Result.SourceManager), template_args[0].getSourceRange());
              llvm::outs() << template_args_str << '\n';
              string new_template_args_str = "";
              if (getrandom::getRandomIndex(1)) {
                new_template_args_str = template_args_str.substr(
                    0, template_args_str.find_last_of(','));
              } else {
                new_template_args_str =
                    template_args_str + ", int";
              }
              new_template_args_str = "/*mut196*/" + new_template_args_str;
              llvm::outs() << new_template_args_str << '\n';
              Rewrite.ReplaceText(
                  CharSourceRange::getTokenRange(template_args[0].getSourceRange()),
                  new_template_args_str);
            }
          }
        }
      }
}
  
void MutatorFrontendAction_196::MutatorASTConsumer_196::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto variadic_function_template_matcher =
        functionTemplateDecl().bind("VariadicFunctionTemplate");
    auto call_expr_matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(variadic_function_template_matcher, &callback);
    matchFinder.addMatcher(call_expr_matcher, &callback);
    matchFinder.matchAST(Context);
}