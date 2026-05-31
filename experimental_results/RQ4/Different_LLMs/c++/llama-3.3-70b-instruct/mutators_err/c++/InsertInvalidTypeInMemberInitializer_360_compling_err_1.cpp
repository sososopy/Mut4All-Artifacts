```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * InsertInvalidTypeInMemberInitializer_360
 */ 
class MutatorFrontendAction_360 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(360)

private:
    class MutatorASTConsumer_360 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_360(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_InsertInvalidTypeInMemberInitializer_360.h"

// ========================================================================================================
#define MUT360_OUTPUT 1

void MutatorFrontendAction_360::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *C = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!C || !Result.Context->getSourceManager().isWrittenInMainFile(
                     C->getLocation()))
        return;
      //Get the source code text of target node
      auto initList = C->getInit(0);
      if (!initList)
        return;
      auto memberVar = initList->getMember();
      if (!memberVar)
        return;
      auto type = memberVar->getType().getAsString();
      //Perform mutation on the source code text by applying string replacement
      std::string invalidType = "InvalidType";
      std::string mutatedType = type + "/*mut360*/" + invalidType;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(memberVar->getSourceRange()), mutatedType);
    }
}
  
void MutatorFrontendAction_360::MutatorASTConsumer_360::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}