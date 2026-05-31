//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_macro_usage_in_constexpr_21
 */ 
class MutatorFrontendAction_21 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(21)

private:
    class MutatorASTConsumer_21 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_21(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_macro_usage_in_constexpr_21.h"

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::ConstantExpr>("ConstExpr")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getExprLoc()))
        return;

      auto *MacroCall = Result.Nodes.getNodeAs<clang::CallExpr>("MacroCall");
      if (!MacroCall)
        return;

      std::string macroText = stringutils::rangetoStr(*(Result.SourceManager), MacroCall->getSourceRange());
      std::string mutatedMacroText = macroText;

      if (MacroCall->getNumArgs() > 1) {
        auto *Arg = MacroCall->getArg(1);
        if (auto *MemberExpr = dyn_cast<clang::MemberExpr>(Arg)) {
          std::string memberText = stringutils::rangetoStr(*(Result.SourceManager), MemberExpr->getSourceRange());
          mutatedMacroText = macroText.replace(macroText.find(memberText), memberText.length(), "*(ptr->member)");
        }
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MacroCall->getSourceRange()), mutatedMacroText);
    }
}
  
void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = constantExpr(hasDescendant(callExpr(callee(functionDecl(hasName("max")))).bind("MacroCall"))).bind("ConstExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}