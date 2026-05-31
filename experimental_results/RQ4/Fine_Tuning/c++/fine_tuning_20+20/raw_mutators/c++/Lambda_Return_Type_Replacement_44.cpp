//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_Return_Type_Replacement_44
 */ 
class MutatorFrontendAction_44 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(44)

private:
    class MutatorASTConsumer_44 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_44(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Lambda_Return_Type_Replacement_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LS = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!LS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LS->getBeginLoc()))
        return;
      if (LS->hasExplicitParameters() == false)
        return;
      auto lambda_content = stringutils::rangetoStr(
          *(Result.SourceManager), LS->getSourceRange());
      auto param_content = stringutils::rangetoStr(
          *(Result.SourceManager), LS->getLambdaClass()->getSourceRange());
      if (lambda_content.find("return") == string::npos)
        return;
      if (param_content.find("...") == string::npos)
        return;
      if (lambda_content.find("return") != string::npos) {
        string fold_content = "";
        if (lambda_content.find("return") != string::npos) {
          fold_content = "return (([a]{}, 0) + ...)";
        }
        llvm::outs() << fold_content << '\n';
        llvm::outs() << lambda_content << '\n';
        llvm::outs() << param_content << '\n';
        stringutils::strReplace(lambda_content, "return ([a]{}, ..., 0)",
                                fold_content);
        llvm::outs() << lambda_content << '\n';
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(LS->getSourceRange()), lambda_content);
    }
}
  
void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}