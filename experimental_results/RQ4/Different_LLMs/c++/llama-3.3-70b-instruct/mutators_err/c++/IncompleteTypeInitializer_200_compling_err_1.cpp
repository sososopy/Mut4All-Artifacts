```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * IncompleteTypeInitializer_200
 */ 
class MutatorFrontendAction_200 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(200)

private:
    class MutatorASTConsumer_200 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_200(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/IncompleteTypeInitializer_200.h"

// ========================================================================================================
#define MUT200_OUTPUT 1

void MutatorFrontendAction_200::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      // Check if the type of the variable is a struct or union
      if (VD->getType()->isStructureOrClassType() || VD->getType()->isUnionType()) {
        // Get the initializer of the variable declaration
        auto initializer = VD->getInit();
        if (initializer) {
          // Perform mutation on the initializer
          std::string newInitializer = "{ 0, 1 }"; // Replace with an incompatible initializer
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(initializer->getSourceRange()), newInitializer);
        }
      }
    }
}
  
void MutatorFrontendAction_200::MutatorASTConsumer_200::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer()).bind("VarDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}