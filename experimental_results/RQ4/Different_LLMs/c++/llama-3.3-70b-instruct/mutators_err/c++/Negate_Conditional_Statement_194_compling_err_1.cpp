//header file
#pragma once
#include "Mutator_base.h"

/**
 * Negate_Conditional_Statement_194
 */ 
class MutatorFrontendAction_Negate_Conditional_Statement_194 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Negate_Conditional_Statement_194)

private:
    class MutatorASTConsumer_Negate_Conditional_Statement_194 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Negate_Conditional_Statement_194(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Negate_Conditional_Statement_194.h"

// ========================================================================================================
#define NEGATE_CONDITIONAL_STATEMENT_194_OUTPUT 1

void MutatorFrontendAction_Negate_Conditional_Statement_194::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto condition = stringutils::rangetoStr(*(Result.SourceManager), MT->getCond()->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto negatedCondition = "!(" + condition + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getCond()->getSourceRange()), negatedCondition);
    }
}

void MutatorFrontendAction_Negate_Conditional_Statement_194::MutatorASTConsumer_Negate_Conditional_Statement_194::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = ifStmt().bind("IfStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}