//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Malformed_Type_Member_Access_310
 */ 
class MutatorFrontendAction_310 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(310)

private:
    class MutatorASTConsumer_310 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_310(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Insert_Malformed_Type_Member_Access_310.h"

// ========================================================================================================
#define MUT310_OUTPUT 1

void MutatorFrontendAction_310::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TN = Result.Nodes.getNodeAs<clang::Type>("TypeNode")) {
      //Filter nodes in header files
      if (!TN || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Result.SourceManager->getSpellingLoc(TN->getTypePtr()->getBeginLoc())))
        return;
      //Get the source code text of target node
      auto typeName = TN->getTypePtr()->getAsString();
      if (typeName.empty())
        return;
      //Perform mutation on the source code text by applying string replacement
      char randomLetter = 'a' + getrandom::getRandomIndex(25);
      string mutatedTypeName = typeName + "." + randomLetter;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(Result.SourceManager->getSpellingLoc(TN->getTypePtr()->getBeginLoc()), typeName.length(), mutatedTypeName);
    }
}
  
void MutatorFrontendAction_310::MutatorASTConsumer_310::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = type().bind("TypeNode");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}