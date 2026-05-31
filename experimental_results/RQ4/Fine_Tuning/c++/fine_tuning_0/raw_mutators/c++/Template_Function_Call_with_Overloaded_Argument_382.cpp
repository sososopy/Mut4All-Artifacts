//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Function_Call_with_Overloaded_Argument_382
 */ 
class MutatorFrontendAction_382 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(382)

private:
    class MutatorASTConsumer_382 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_382(Rewriter &R) : TheRewriter(R) {}
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
        
    };
};

//source file
#include "../include/Template_Function_Call_with_Overloaded_Argument_382.h"

// ========================================================================================================
#define MUT382_OUTPUT 1

void MutatorFrontendAction_382::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("TemplateFunctionCall")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->getNumArgs() == 0)
        return;
      auto callee = MT->getDirectCallee();
      if (!callee->isTemplateInstantiation())
        return;
      auto args = MT->getArgs();
      auto arg = args[getrandom::getRandomIndex(args.size() - 1)];
      auto arg_source = stringutils::rangetoStr(*(Result.SourceManager),
                                                arg->getSourceRange());
      arg_source = "/*mut382*/" + arg_source;
      auto arg_type = arg->getType();
      if (arg_type->isIntegerType()) {
        arg_source = "[](){return 0;}";
      } else if (arg_type->isBooleanType()) {
        arg_source = "[](){return false;}";
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(arg->getSourceRange()),
                          arg_source);
    }
}
  
void MutatorFrontendAction_382::MutatorASTConsumer_382::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr().bind("TemplateFunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}