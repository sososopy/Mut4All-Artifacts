//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Malformed_Builtin_Operator_In_Return_366
 */ 
class MutatorFrontendAction_366 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(366)

private:
    class MutatorASTConsumer_366 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_366(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> builtinOps = {"__array_extent", "__builtin_offsetof", "__underlying_type"};
    };
};

//source file
#include "../include/Mutator_Insert_Malformed_Builtin_Operator_In_Return_366.h"

// ========================================================================================================
#define MUT366_OUTPUT 1

void MutatorFrontendAction_366::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>("ReturnStmt")) {
      //Filter nodes in header files
      if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RS->getLocation()))
        return;
      //Get the source code text of target node
      auto returnExpr = RS->getRetValue();
      if (!returnExpr) return;
      auto sourceRange = CharSourceRange::getTokenRange(RS->getSourceRange());
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      //Perform mutation on the source code text by applying string replacement
      size_t opIndex = getrandom::getRandomIndex(builtinOps.size() - 1);
      std::string malformedOp = builtinOps[opIndex];
      std::string mutatedText = "return " + malformedOp + "(int, );";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(sourceRange, mutatedText);
    }
}
  
void MutatorFrontendAction_366::MutatorASTConsumer_366::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = returnStmt().bind("ReturnStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}