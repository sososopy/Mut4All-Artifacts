```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * ArrayInitializerMutation_470
 */ 
class MutatorFrontendAction_470 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(470)

private:
    class MutatorASTConsumer_470 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_470(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ArrayInitializerMutation_470.h"

// ========================================================================================================
#define MUT470_OUTPUT 1

void MutatorFrontendAction_470::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (VD->getType()->isArrayType()) {
        std::string mutatedDeclaration = declaration;
        // Replace array initialization with pointer initialization
        size_t pos = mutatedDeclaration.find("=");
        if (pos != std::string::npos) {
          mutatedDeclaration.replace(pos, 1, "*");
        }
        // Replace array type with pointer type
        pos = mutatedDeclaration.find("[");
        if (pos != std::string::npos) {
          mutatedDeclaration.replace(pos, 1, "*");
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), mutatedDeclaration);
      }
    }
}
  
void MutatorFrontendAction_470::MutatorASTConsumer_470::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(arrayType())).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}