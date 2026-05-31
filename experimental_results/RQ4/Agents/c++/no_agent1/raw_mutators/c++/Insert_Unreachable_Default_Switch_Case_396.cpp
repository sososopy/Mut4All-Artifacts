//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Default_Switch_Case_396
 */ 
class MutatorFrontendAction_396 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(396)

private:
    class MutatorASTConsumer_396 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_396(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Unreachable_Default_Switch_Case_396.h"

// ========================================================================================================
#define MUT396_OUTPUT 1

void MutatorFrontendAction_396::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ST = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
      //Filter nodes in header files
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto switchBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                ST->getBody()->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string unreachableDefault = "\n/*mut396*/default: __builtin_unreachable();\n";
      if (switchBody.find("default:") == std::string::npos) {
        switchBody.insert(switchBody.rfind('}'), unreachableDefault);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getBody()->getSourceRange()), switchBody);
    }
}
  
void MutatorFrontendAction_396::MutatorASTConsumer_396::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = switchStmt().bind("SwitchStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}