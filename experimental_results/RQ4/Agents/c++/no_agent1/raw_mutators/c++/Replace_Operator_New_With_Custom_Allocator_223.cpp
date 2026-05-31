//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Operator_New_With_Custom_Allocator_223
 */ 
class MutatorFrontendAction_223 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(223)

private:
    class MutatorASTConsumer_223 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_223(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_operator_new_with_custom_allocator_223.h"

// ========================================================================================================
#define MUT223_OUTPUT 1

void MutatorFrontendAction_223::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NE = Result.Nodes.getNodeAs<clang::CXXNewExpr>("NewExpr")) {
      //Filter nodes in header files
      if (!NE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NE->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto newExprText = stringutils::rangetoStr(*(Result.SourceManager), NE->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string customAllocator = "CustomAllocator::allocate";
      std::string mutatedExpr = newExprText;
      size_t pos = mutatedExpr.find("new");
      if (pos != std::string::npos) {
          mutatedExpr.replace(pos, 3, customAllocator);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(NE->getSourceRange()), mutatedExpr);
    }
}
  
void MutatorFrontendAction_223::MutatorASTConsumer_223::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxNewExpr().bind("NewExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}