//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Template_Argument_Deduction_Call_Expression_32
 */ 
class MutatorFrontendAction_32 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(32)

private:
    class MutatorASTConsumer_32 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_32(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Transform_Template_Argument_Deduction_Call_Expression_32.h"

// ========================================================================================================
#define MUT32_OUTPUT 1

void MutatorFrontendAction_32::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;

      if (!MT->getDirectCallee())
        return;
      if (!MT->getDirectCallee()->isTemplateInstantiation())
        return;
      if (MT->getNumArgs() == 0)
        return;
      auto arg = MT->getArg(0);
      auto arg_type = arg->getType();
      auto arg_str = stringutils::rangetoStr(*(Result.SourceManager),
                                             arg->getSourceRange());
      auto callee_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getCallee()->getSourceRange());
      if (arg_type->isFunctionPointerType()) {
        arg_str = "/*mut32*/std::forward<decltype(" + arg_str + ")>(" + arg_str +
                  ")";
      } else if (arg_type->isFunctionPointerType()) {
        arg_str = "/*mut32*/std::make_tuple(" + arg_str + ")";
      } else if (arg_type->isLambdaType()) {
        arg_str = "/*mut32*/[&](auto&&){}";
      } else {
        return;
      }
      auto call_str = callee_str + "(" + arg_str + ")";
      llvm::outs() << call_str << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          call_str);
    }
}
  
void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}