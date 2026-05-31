//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_Block_422
 */ 
class MutatorFrontendAction_422 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(422)

private:
    class MutatorASTConsumer_422 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_422(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_code_block_422.h"

// ========================================================================================================
#define MUT422_OUTPUT 1

void MutatorFrontendAction_422::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::CompoundStmt>("FunctionBody")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLBracLoc()))
        return;

      //Get the source code text of target node
      auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), FS->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string unreachableCode = "\n/*mut422*/if (false) { volatile int x = 0; while (true) { ++x; } }\n";
      bodyText.insert(bodyText.find("{") + 1, unreachableCode);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getSourceRange()), bodyText);
    }
}
  
void MutatorFrontendAction_422::MutatorASTConsumer_422::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = compoundStmt(hasParent(functionDecl())).bind("FunctionBody");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}