//header file
#pragma once
#include "Mutator_base.h"

/**
 * Annotated_Dependent_Lambda_Transformation_487
 */ 
class MutatorFrontendAction_487 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(487)

private:
    class MutatorASTConsumer_487 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_487(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Annotated_Dependent_Lambda_Transformation_487.h"

// ========================================================================================================
#define MUT487_OUTPUT 1

void MutatorFrontendAction_487::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("DependentLambdas")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      auto DL_str = stringutils::rangetoStr(*(Result.SourceManager),
                                            DL->getSourceRange());
      llvm::outs() << DL_str << '\n';
      if (DL->hasExplicitTemplateArgumentList() == true) {
        llvm::outs() << "has explicit template args\n";
        return;
      }
      if (DL->hasExplicitParameters() == false) {
        DL_str.insert(1, "=");
        Rewrite.ReplaceText(DL->getSourceRange(), DL_str);
        return;
      }
      DL_str.insert(1, "=");
      Rewrite.ReplaceText(DL->getSourceRange(), DL_str);
    }
}
  
void MutatorFrontendAction_487::MutatorASTConsumer_487::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasAncestor(functionTemplateDecl())).bind("DependentLambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}