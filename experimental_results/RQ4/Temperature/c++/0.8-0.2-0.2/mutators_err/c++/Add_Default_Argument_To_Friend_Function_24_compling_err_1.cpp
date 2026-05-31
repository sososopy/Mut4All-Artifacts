```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * add_default_argument_to_friend_function_24
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
        std::map<const clang::FunctionDecl*, int> defaultArgs;
    };
};

//source file
#include "../include/add_default_argument_to_friend_function_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FriendFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      // Check if the function is a friend function inside a template struct
      if (FD->getFriendObjectKind() != clang::Decl::FOK_None && FD->getDescribedFunctionTemplate()) {
          if (FD->getNumParams() > 1 && !FD->getParamDecl(1)->hasDefaultArg()) {
              // Add default argument or modify if already exists
              int defaultValue = 42;
              if (defaultArgs.find(FD) != defaultArgs.end()) {
                  defaultValue = defaultArgs[FD] + 1; // Modify existing
              }
              defaultArgs[FD] = defaultValue;

              //Perform mutation on the source code text by applying string replacement
              auto paramRange = FD->getParamDecl(1)->getSourceRange();
              std::string paramText = stringutils::rangetoStr(*(Result.SourceManager), paramRange);
              std::string newParamText = paramText + " = " + std::to_string(defaultValue);

              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(paramRange), newParamText);
          }
      }
    }
}
  
void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isFriend(), isInTemplateStruct()).bind("FriendFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
```