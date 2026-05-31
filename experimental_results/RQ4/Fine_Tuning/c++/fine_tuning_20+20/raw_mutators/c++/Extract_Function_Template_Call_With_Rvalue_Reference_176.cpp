//header file
#pragma once
#include "Mutator_base.h"

/**
 * Extract_Function_Template_Call_With_Rvalue_Reference_176
 */ 
class MutatorFrontendAction_176 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(176)

private:
    class MutatorASTConsumer_176 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_176(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Extract_Function_Template_Call_With_Rvalue_Reference_176.h"

// ========================================================================================================
#define MUT176_OUTPUT 1

void MutatorFrontendAction_176::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("FuncCall")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      if (!MT->getDirectCallee())
        return;
      if (!MT->getDirectCallee()->isFunctionTemplateSpecialization())
        return;

      auto args = MT->getArgs();
      for (auto arg : args) {
        if (arg->isRValue()) {
          content = "/*mut176*/auto temp = " + content + ";\n";
          content += "temp";
          break;
        }
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_176::MutatorASTConsumer_176::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr().bind("FuncCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}