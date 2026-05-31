//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Unreachable_Default_Case_43
 */ 
class MutatorFrontendAction_43 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(43)

private:
    class MutatorASTConsumer_43 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_43(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/add_unreachable_default_case_43.h"

// ========================================================================================================
#define MUT43_OUTPUT 1

void MutatorFrontendAction_43::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SS = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
      //Filter nodes in header files
      if (!SS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SS->getSwitchLoc()))
        return;
      
      //Get the source code text of target node
      auto switchBody = stringutils::rangetoStr(*(Result.SourceManager), SS->getBody()->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string defaultCase = "\n/*mut43*/default: __builtin_unreachable();\n";
      
      // Check if a default case already exists
      if (switchBody.find("default:") == std::string::npos) {
        if (switchBody.rfind('}') != std::string::npos) {
          switchBody.insert(switchBody.rfind('}'), defaultCase);
        }
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(SS->getBody()->getSourceRange()), switchBody);
      }
    }
}
  
void MutatorFrontendAction_43::MutatorASTConsumer_43::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = switchStmt().bind("SwitchStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}