//header file
#pragma once
#include "Mutator_base.h"

/**
 * Function_Template_Instantiation_With_String_Argument_98
 */ 
class MutatorFrontendAction_98 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(98)

private:
    class MutatorASTConsumer_98 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_98(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::FunctionTemplateDecl *> cur_templates;
    };
};

//source file
#include "../include/function_template_instantiation_with_string_argument_98.h"

// ========================================================================================================
#define MUT98_OUTPUT 1

void MutatorFrontendAction_98::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("Templates")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      cur_templates.push_back(FT);
    } else if (auto *CI = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      if (!CI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CI->getBeginLoc()))
        return;
      auto callee = CI->getDirectCallee();
      if (callee == nullptr)
        return;
      auto callee_name = callee->getNameAsString();
      llvm::outs() << callee_name << '\n';
      auto args = CI->getArgs();
      for (auto arg : args) {
        if (arg->getType()->isCharType() || arg->getType()->isChar8Type()) {
          auto arg_str =
              stringutils::rangetoStr(*(Result.SourceManager), arg->getSourceRange());
          llvm::outs() << arg_str << '\n';
          for (auto ft : cur_templates) {
            if (ft->getNameAsString() == callee_name) {
              auto specs = ft->specializations();
              for (auto spec : specs) {
                auto args = spec->getTemplateArgs();
                for (size_t i = 0; i < args.size(); ++i) {
                  auto cur_arg = args[i];
                  if (cur_arg.getKind() ==
                      clang::TemplateArgument::ArgKind::Integral) {
                    llvm::outs() << cur_arg.getAsIntegral().toString(10) << '\n';
                    if (cur_arg.getAsIntegral().toString(10) == arg_str) {
                      auto instantiation =
                          stringutils::rangetoStr(*(Result.SourceManager),
                                                  CI->getSourceRange());
                      instantiation = "/*mut98*/" + instantiation;
                      Rewrite.ReplaceText(
                          CharSourceRange::getTokenRange(CI->getSourceRange()),
                          instantiation);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_98::MutatorASTConsumer_98::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("Templates");
    auto call_matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(call_matcher, &callback);
    matchFinder.matchAST(Context);
}