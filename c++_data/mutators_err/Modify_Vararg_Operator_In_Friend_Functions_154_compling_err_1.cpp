//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Vararg_Operator_In_Friend_Functions_154
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
    };
};

//source file
#include "../include/modify_vararg_operator_in_friend_functions_154.h"

// ========================================================================================================
#define MUT154_OUTPUT 1

void MutatorFrontendAction_154::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FriendFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      // Check if the function is a friend function with vararg
      if (FD->isFriend() && FD->isVariadic()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        auto pos = declaration.find("...");
        if (pos != std::string::npos) {
          declaration.replace(pos, 3, "int");
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_154::MutatorASTConsumer_154::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isFriend(), isVariadic()).bind("FriendFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}