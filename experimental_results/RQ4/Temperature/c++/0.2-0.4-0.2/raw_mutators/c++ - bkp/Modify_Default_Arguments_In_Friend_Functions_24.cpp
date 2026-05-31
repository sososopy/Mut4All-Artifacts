//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_default_arguments_in_friend_functions_24
 */ 
class MutatorFrontendAction_24 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(24)

private:
    class MutatorASTConsumer_24 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_24(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_default_arguments_in_friend_functions_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FriendFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      // Check if the function is a friend and has default arguments
      if (FD->isFriend() && FD->getNumParams() > 0) {
        for (unsigned i = 0; i < FD->getNumParams(); ++i) {
          if (FD->getParamDecl(i)->hasDefaultArg()) {
            //Get the source code text of target node
            auto paramRange = FD->getParamDecl(i)->getSourceRange();
            auto defaultArgRange = FD->getParamDecl(i)->getDefaultArgRange();
            //Perform mutation on the source code text by applying string replacement
            Rewrite.RemoveText(defaultArgRange);
            //Replace the original AST node with the mutated one
          }
        }
      }
    }
}
  
void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isFriend(), hasAnyParameter(hasDefaultArgument())).bind("FriendFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}