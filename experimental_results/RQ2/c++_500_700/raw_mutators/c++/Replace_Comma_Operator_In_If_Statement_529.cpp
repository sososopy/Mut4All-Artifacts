```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * replace_comma_operator_in_if_statement_529
 */ 
class MutatorFrontendAction_529 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(529)

private:
    class MutatorASTConsumer_529 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_529(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_comma_operator_in_if_statement_529.h"

// ========================================================================================================
#define MUT529_OUTPUT 1

void MutatorFrontendAction_529::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *IfStmt = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt")) {
      //Filter nodes in header files
      if (!IfStmt || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IfStmt->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto conditionRange = IfStmt->getCond()->getSourceRange();
      std::string conditionText = Lexer::getSourceText(CharSourceRange::getTokenRange(conditionRange), 
                                                       *Result.SourceManager, 
                                                       Result.Context->getLangOpts()).str();

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCondition = "(x + 1) && (x == 0)";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(conditionRange, mutatedCondition);
    }
}
  
void MutatorFrontendAction_529::MutatorASTConsumer_529::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = ifStmt(hasCondition(anyOf(binaryOperator(hasOperatorName(",")), 
                                                         compoundStmt()))).bind("ifStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
```