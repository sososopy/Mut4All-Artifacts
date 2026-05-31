//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decltype_Lambda_With_Sizeof_71
 */ 
class MutatorFrontendAction_71 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(71)

private:
    class MutatorASTConsumer_71 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_71(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Decltype_Lambda_With_Sizeof_71.h"

// ========================================================================================================
#define MUT71_OUTPUT 1

void MutatorFrontendAction_71::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DS = Result.Nodes.getNodeAs<clang::DecltypeTypeLoc>("DecltypeLoc")) {
      //Filter nodes in header files
      if (!DS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DS->getBeginLoc()))
        return;
      
      // Get the underlying expression
      auto *Expr = DS->getUnderlyingExpr();
      if (!Expr) return;
      
      // Check if the expression is a lambda
      if (!llvm::isa<clang::LambdaExpr>(Expr)) return;
      
      //Get the source code text of target node
      auto SourceRange = DS->getSourceRange();
      std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), SourceRange);
      
      // Extract just the lambda expression part
      auto LambdaRange = Expr->getSourceRange();
      std::string LambdaText = stringutils::rangetoStr(*(Result.SourceManager), LambdaRange);
      
      //Perform mutation on the source code text by applying string replacement
      std::string MutatedText = "decltype(sizeof(" + LambdaText + "))";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), MutatedText);
    }
}
  
void MutatorFrontendAction_71::MutatorASTConsumer_71::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = decltypeTypeLoc().bind("DecltypeLoc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}