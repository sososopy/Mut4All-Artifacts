//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Variable_Argument_Order_154
 */ 
class MutatorFrontendAction_154 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(154)

private:
    class MutatorASTConsumer_154 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_154(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Modify_Variable_Argument_Order_154.h"

// ========================================================================================================
#define MUT154_OUTPUT 1

void MutatorFrontendAction_154::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *InitList = Result.Nodes.getNodeAs<clang::InitListExpr>("InitList")) {
      //Filter nodes in header files
      if (!InitList || !Result.Context->getSourceManager().isWrittenInMainFile(
                     InitList->getLocation()))
        return;
      //Get the source code text of target node
      auto initListText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   InitList->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::vector<std::string> args;
      size_t pos = 0;
      while ((pos = initListText.find(',')) != std::string::npos) {
        args.push_back(initListText.substr(0, pos));
        initListText.erase(0, pos + 1);
      }
      args.push_back(initListText);
      // Reverse the order of arguments
      std::reverse(args.begin(), args.end());
      std::string mutatedInitListText;
      for (const auto &arg : args) {
        mutatedInitListText += arg + ",";
      }
      mutatedInitListText.pop_back(); // Remove the trailing comma
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(InitList->getSourceRange()), mutatedInitListText);
    }
}
  
void MutatorFrontendAction_154::MutatorASTConsumer_154::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    ExprMatcher matcher = initListExpr().bind("InitList");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}