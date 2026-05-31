//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Lambda_Return_Type_To_Function_Type_199
 */ 
class MutatorFrontendAction_199 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(199)

private:
    class MutatorASTConsumer_199 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_199(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Change_Lambda_Return_Type_To_Function_Type_199.h"

// ========================================================================================================
#define MUT199_OUTPUT 1

void MutatorFrontendAction_199::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LD = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LD->getBeginLoc()))
        return;
      if (LD->hasExplicitParameters() == false)
        return;
      auto lambda_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                 LD->getSourceRange());
      if (lambda_text.find("->") == string::npos)
        return;
      llvm::outs() << lambda_text << '\n';
      auto arrow_pos = lambda_text.find("->");
      auto lbrace_pos = lambda_text.find("{");
      if (arrow_pos > lbrace_pos)
        return;
      auto return_type = lambda_text.substr(arrow_pos + 2, lbrace_pos - arrow_pos - 2);
      llvm::outs() << return_type << '\n';
      auto new_type = return_type + "(int)";
      lambda_text.replace(arrow_pos + 2, lbrace_pos - arrow_pos - 2, new_type);
      llvm::outs() << lambda_text << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LD->getSourceRange()), lambda_text);
    }
}
  
void MutatorFrontendAction_199::MutatorASTConsumer_199::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}