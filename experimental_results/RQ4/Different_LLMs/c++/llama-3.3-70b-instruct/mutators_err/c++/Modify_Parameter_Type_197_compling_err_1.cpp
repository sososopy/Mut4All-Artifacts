```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Parameter_Type_197
 */ 
class MutatorFrontendAction_197 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(197)

private:
    class MutatorASTConsumer_197 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_197(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::Type *> existingTypes;
    };
};

//source file
#include "../include/Mutator_Modify_Parameter_Type_197.h"

// ========================================================================================================
#define MUT197_OUTPUT 1

void MutatorFrontendAction_197::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto params = FD->params();
      for (auto param : params) {
        //Record existing types in the seed program
        if (std::find(existingTypes.begin(), existingTypes.end(), param->getType()) == existingTypes.end()) {
          existingTypes.push_back(param->getType());
        }
      }
      //Perform mutation on the source code text by applying string replacement
      for (auto param : params) {
        int choice = getrandom::getRandomIndex(existingTypes.size() - 1);
        if (choice >= 0) {
          auto newType = existingTypes[choice];
          if (newType != param->getType()) {
            std::string newParamStr = param->getNameAsString() + " : " + newType->getAsString();
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(param->getSourceRange()), newParamStr);
          }
        }
      }
    }
}

void MutatorFrontendAction_197::MutatorASTConsumer_197::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}