//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Array_Size_366
 */ 
class MutatorFrontendAction_366 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(366)

private:
    class MutatorASTConsumer_366 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_366(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Array_Size_366.h"

// ========================================================================================================
#define MUT366_OUTPUT 1

void MutatorFrontendAction_366::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (VD->getType()->isArrayType()) {
        auto arrayType = VD->getType()->getAsArrayType();
        if (arrayType->getSize() != nullptr) {
          auto newSize = "";
          declaration.replace(declaration.find("[" + arrayType->getSize()->getAsString() + "]"), 
                              arrayType->getSize()->getAsString().length() + 2, newSize);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_366::MutatorASTConsumer_366::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(arrayType())).bind("VarDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}