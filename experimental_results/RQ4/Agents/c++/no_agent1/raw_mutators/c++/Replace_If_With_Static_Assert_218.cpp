//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_If_With_Static_Assert_218
 */ 
class MutatorFrontendAction_218 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(218)

private:
    class MutatorASTConsumer_218 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_218(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_if_with_static_assert_218.h"

// ========================================================================================================
#define MUT218_OUTPUT 1

void MutatorFrontendAction_218::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *IF = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt")) {
      //Filter nodes in header files
      if (!IF || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IF->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto condition = stringutils::rangetoStr(*(Result.SourceManager),
                                               IF->getCond()->getSourceRange());
      auto thenStmt = stringutils::rangetoStr(*(Result.SourceManager),
                                              IF->getThen()->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string staticAssertText = "static_assert(" + condition + ", \"Condition failed\");\n";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(IF->getSourceRange()), staticAssertText + thenStmt);
    }
}
  
void MutatorFrontendAction_218::MutatorASTConsumer_218::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = ifStmt().bind("IfStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}