```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Conditional_With_Loop_408
 */ 
class MutatorFrontendAction_408 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(408)

private:
    class MutatorASTConsumer_408 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_408(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Conditional_With_Loop_408.h"

// ========================================================================================================
#define MUT408_OUTPUT 1

void MutatorFrontendAction_408::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *If = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt")) {
      //Filter nodes in header files
      if (!If || !Result.Context->getSourceManager().isWrittenInMainFile(
                     If->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto condition = stringutils::rangetoStr(*(Result.SourceManager), If->getCond()->getSourceRange());
      auto thenStmt = stringutils::rangetoStr(*(Result.SourceManager), If->getThen()->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCode = "while (" + condition + ") {\n" + thenStmt + "\nbreak;\n}";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(If->getSourceRange()), mutatedCode);
    }
    else if (auto *Switch = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
      //Filter nodes in header files
      if (!Switch || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Switch->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto cond = stringutils::rangetoStr(*(Result.SourceManager), Switch->getCond()->getSourceRange());
      auto body = stringutils::rangetoStr(*(Result.SourceManager), Switch->getBody()->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCode = "while (" + cond + ") {\n" + body + "\nbreak;\n}";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Switch->getSourceRange()), mutatedCode);
    }
    else if (auto *Ternary = Result.Nodes.getNodeAs<clang::ConditionalOperator>("TernaryOp")) {
      //Filter nodes in header files
      if (!Ternary || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Ternary->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto cond = stringutils::rangetoStr(*(Result.SourceManager), Ternary->getCond()->getSourceRange());
      auto thenExpr = stringutils::rangetoStr(*(Result.SourceManager), Ternary->getTrueExpr()->getSourceRange());
      auto elseExpr = stringutils::rangetoStr(*(Result.SourceManager), Ternary->getFalseExpr()->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCode = "while (" + cond + ") {\n" + thenExpr + "\nbreak;\n} else {\n" + elseExpr + "\n}";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Ternary->getSourceRange()), mutatedCode);
    }
}
  
void MutatorFrontendAction_408::MutatorASTConsumer_408::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StmtMatcher matcher1 = ifStmt().bind("IfStmt");
    StmtMatcher matcher2 = switchStmt().bind("SwitchStmt");
    StmtMatcher matcher3 = conditionalOperator().bind("TernaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.addMatcher(matcher3, &callback);
    matchFinder.matchAST(Context);
}