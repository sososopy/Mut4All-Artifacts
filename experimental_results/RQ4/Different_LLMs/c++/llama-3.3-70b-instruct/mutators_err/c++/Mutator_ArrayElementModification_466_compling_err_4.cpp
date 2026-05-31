//header file
#pragma once
#include "Mutator_base.h"

/**
 * ArrayElementModification_466
 */ 
class MutatorFrontendAction_466 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(466)

private:
    class MutatorASTConsumer_466 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_466(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_ArrayElementModification_466.h"

// ========================================================================================================
#define MUT466_OUTPUT 1

void MutatorFrontendAction_466::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VA = Result.Nodes.getNodeAs<clang::VarDecl>("VarArray")) {
      //Filter nodes in header files
      if (!VA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VA->getLocation()))
        return;
      //Get the source code text of target node
      auto init = VA->getInit();
      if (init && init->getInitList()) {
        auto initList = init->getInitList();
        if (initList->getNumInits() > 0) {
          //Perform mutation on the source code text by applying string replacement
          int choice = getrandom::getRandomIndex(1);
          if (choice) {
            // Replace the initializer of the selected array element with a default or zero initializer
            auto initExpr = initList->getInit(0);
            auto replacedInit = clang::IntegerLiteral::Create(Result.Context, clang::APValue(0), initExpr->getLocStart());
            Rewrite.ReplaceText(initExpr->getSourceRange(), "0");
          } else {
            // Remove the initializer if it exists
            auto initExpr = initList->getInit(0);
            Rewrite.RemoveText(initExpr->getSourceRange());
          }
        }
      } else {
        // If the array is not initialized or partially initialized, select an uninitialized element and initialize it with a value
        auto type = VA->getType();
        if (type->isArrayType()) {
          auto arrayType = type->getAsArrayTypeUnsafe();
          auto elementType = arrayType->getElementType();
          auto numElements = arrayType->getSize(Result.Context);
          if (numElements) {
            // Initialize an uninitialized element with a value
            Rewrite.InsertTextAfterToken(VA->getEndLoc(), " = {0};");
          }
        }
      }
    }
}

void MutatorFrontendAction_466::MutatorASTConsumer_466::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(arrayType())).bind("VarArray");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}