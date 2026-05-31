//header file
#pragma once
#include "Mutator_base.h"

/**
 * Type_Mismatch_Comparison_237
 */ 
class MutatorFrontendAction_Type_Mismatch_Comparison_237 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Type_Mismatch_Comparison_237)

private:
    class MutatorASTConsumer_Type_Mismatch_Comparison_237 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Type_Mismatch_Comparison_237(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Type_Mismatch_Comparison_237.h"

// ========================================================================================================
#define MUT237_OUTPUT 1

void MutatorFrontendAction_Type_Mismatch_Comparison_237::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::BinaryOperator>(("Comparison"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto LHS = MT->getLHS()->getType().getAsString();
      auto RHS = MT->getRHS()->getType().getAsString();
      //Perform mutation on the source code text by applying string replacement
      if (LHS != RHS) {
        // Introduce a type mismatch by replacing one of the operands with a different type
        std::string mutatedCode;
        if (getrandom::getRandomIndex(1)) {
          // Replace LHS with a float
          mutatedCode = "static_cast<float>(" + stringutils::rangetoStr(*(Result.SourceManager), MT->getLHS()->getSourceRange()) + ")";
        } else {
          // Replace RHS with a float
          mutatedCode = stringutils::rangetoStr(*(Result.SourceManager), MT->getLHS()->getSourceRange()) + " > static_cast<float>(" + stringutils::rangetoStr(*(Result.SourceManager), MT->getRHS()->getSourceRange()) + ")";
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedCode);
      }
    }
}
  
void MutatorFrontendAction_Type_Mismatch_Comparison_237::MutatorASTConsumer_Type_Mismatch_Comparison_237::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = binaryOperator(hasOperatorName(">")).bind("Comparison");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}