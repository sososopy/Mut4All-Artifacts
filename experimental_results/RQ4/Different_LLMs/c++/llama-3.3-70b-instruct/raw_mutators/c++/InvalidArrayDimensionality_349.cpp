//header file
#pragma once
#include "Mutator_base.h"

/**
 * InvalidArrayDimensionality_349
 */ 
class MutatorFrontendAction_349 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(349)

private:
    class MutatorASTConsumer_349 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_349(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_InvalidArrayDimensionality_349.h"

// ========================================================================================================
#define MUT349_OUTPUT 1

void MutatorFrontendAction_349::Callback::run(const MatchFinder::MatchResult &Result) {
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
        auto arrayType = VD->getType()->getAsArrayTypeUnsafe();
        auto size = arrayType->getSize();
        if (size) {
          int newSize = -1; // invalid size
          declaration.replace(declaration.find(std::to_string(size->getSExtValue())),
                            std::to_string(size->getSExtValue()).length(),
                            std::to_string(newSize));
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_349::MutatorASTConsumer_349::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(arrayType())).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}