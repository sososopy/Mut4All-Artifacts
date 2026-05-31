//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Break_97
 */ 
class MutatorFrontendAction_97 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(97)

private:
    class MutatorASTConsumer_97 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_97(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_break_97.h"

// ========================================================================================================
#define MUT97_OUTPUT 1

void MutatorFrontendAction_97::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::ForStmt>("ForLoop")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getForLoc()))
        return;

      //Get the source code text of target node
      auto loopBody = stringutils::rangetoStr(*(Result.SourceManager), FS->getBody()->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string unreachableBreak = "\n/*mut97*/if(false) { break; }\n";
      loopBody.insert(loopBody.find("{") + 1, unreachableBreak);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getBody()->getSourceRange()), loopBody);
    }
}
  
void MutatorFrontendAction_97::MutatorASTConsumer_97::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = forStmt().bind("ForLoop");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}