```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Pointer_Assignment_242
 */ 
class MutatorFrontendAction_242 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(242)

private:
    class MutatorASTConsumer_242 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_242(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Function_Pointer_Assignment_242.h"

// ========================================================================================================
#define MUT242_OUTPUT 1

void MutatorFrontendAction_242::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      // Check if the variable is declared with decltype(auto)
      if (MT->getType()->isDecltypeAutoType()) {
        // Check if the initializer is a function pointer
        if (auto *init = MT->getInit()) {
          if (init->getType()->isPointerType()) {
            if (auto *pointeeType = init->getType()->getPointeeType()) {
              if (pointeeType->isFunctionPointerType()) {
                // Perform mutation on the source code text by applying string replacement
                // Replace decltype(auto) with auto
                declaration.replace(declaration.find("decltype(auto)"), 13, "auto");
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_242::MutatorASTConsumer_242::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(decltypeAutoType())).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}