//header file
#pragma once
#include "Mutator_base.h"

/**
 * InsertLambdaAnnotation_487
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/InsertLambdaAnnotation_487.h"

// ========================================================================================================
#define MUT487_OUTPUT 1

void MutatorFrontendAction_487::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!LT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getLocation()))
        return;
      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager),
                                               LT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t paramEnd = lambdaText.find(')');
      if (paramEnd != string::npos) {
        lambdaText.insert(paramEnd + 1, " [[clang::annotate_type(\"annotation\")]]");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LT->getSourceRange()), lambdaText);
    }
}
  
void MutatorFrontendAction_487::MutatorASTConsumer_487::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}