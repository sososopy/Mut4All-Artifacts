//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decimal_Literal_With_Braced_Initializer_In_Return_Statements_21
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Decimal_Literal_With_Braced_Initializer_In_Return_Statements_21.h"

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>("ReturnStmt")) {
      //Filter nodes in header files
      if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RS->getLocation()))
        return;
      //Get the source code text of target node
      auto *RetExpr = RS->getRetValue();
      if (!RetExpr)
        return;
      if (auto *FPL = dyn_cast<FloatingLiteral>(RetExpr)) {
        //(optional)Record the node information to be used in the mutation process
        std::string LiteralStr = stringutils::rangetoStr(*(Result.SourceManager), FPL->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string MutatedStr = "{" + LiteralStr + "}";
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FPL->getSourceRange()), MutatedStr);
      }
    }
}
  
void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = returnStmt(hasReturnValue(hasDescendant(floatingLiteral()))).bind("ReturnStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}