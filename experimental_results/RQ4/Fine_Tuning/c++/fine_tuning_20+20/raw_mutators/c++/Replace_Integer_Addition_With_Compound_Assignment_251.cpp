//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Integer_Addition_With_Compound_Assignment_251
 */ 
class MutatorFrontendAction_251 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(251)

private:
    class MutatorASTConsumer_251 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_251(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Replace_Integer_Addition_With_Compound_Assignment_251.h"

// ========================================================================================================
#define MUT251_OUTPUT 1

void MutatorFrontendAction_251::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getBeginLoc()))
        return;
      if (BO->getOpcode() != BO_Add)
        return;
      if (!BO->getType()->isIntegerType())
        return;
      auto lhs = BO->getLHS();
      auto rhs = BO->getRHS();
      if (!lhs->isLValue())
        return;
      auto lhs_text = stringutils::rangetoStr(*(Result.SourceManager),
                                              lhs->getSourceRange());
      auto rhs_text = stringutils::rangetoStr(*(Result.SourceManager),
                                              rhs->getSourceRange());
      auto bo_text = stringutils::rangetoStr(*(Result.SourceManager),
                                             BO->getSourceRange());
      auto mut_text = lhs_text + "; " + lhs_text + " += " + rhs_text;
      llvm::outs() << bo_text << " -> " << mut_text << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), mut_text);
    }
}
  
void MutatorFrontendAction_251::MutatorASTConsumer_251::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = binaryOperator().bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}