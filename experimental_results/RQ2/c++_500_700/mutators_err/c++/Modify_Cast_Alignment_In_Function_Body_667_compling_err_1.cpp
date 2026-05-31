//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_cast_alignment_in_function_body_667
 */ 
class MutatorFrontendAction_667 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(667)

private:
    class MutatorASTConsumer_667 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_667(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_cast_alignment_in_function_body_667.h"

// ========================================================================================================
#define MUT667_OUTPUT 1

void MutatorFrontendAction_667::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CStyleCastExpr>("CastExpr")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;

      auto &SM = *Result.SourceManager;
      auto &LangOpts = Result.Context->getLangOpts();
      auto typeStr = CE->getTypeAsWritten().getAsString();
      std::string newTypeStr = "alignas(8) " + typeStr;
      
      auto range = CharSourceRange::getTokenRange(CE->getTypeInfoAsWritten()->getSourceRange());
      Rewrite.ReplaceText(range, newTypeStr);
    }
}
  
void MutatorFrontendAction_667::MutatorASTConsumer_667::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cStyleCastExpr(hasType(pointerType())).bind("CastExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}