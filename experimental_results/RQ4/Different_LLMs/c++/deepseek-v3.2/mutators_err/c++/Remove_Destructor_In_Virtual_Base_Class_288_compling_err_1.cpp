//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Destructor_In_Virtual_Base_Class_288
 */ 
class MutatorFrontendAction_288 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(288)

private:
    class MutatorASTConsumer_288 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_288(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_288.h"

// ========================================================================================================
#define MUT288_OUTPUT 1

void MutatorFrontendAction_288::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassWithVirtualBase")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      //Check if class has virtual base
      bool hasVirtualBase = false;
      for (auto base : MT->bases()) {
        if (base.isVirtual()) {
          hasVirtualBase = true;
          break;
        }
      }
      if (!hasVirtualBase)
        return;
      //Get destructor
      auto dtor = MT->getDestructor();
      if (!dtor || dtor->isImplicit())
        return;
      //Get the source code text of target node
      auto dtorRange = dtor->getSourceRange();
      if (dtorRange.isInvalid())
        return;
      //Perform mutation on the source code text by applying string replacement
      //Remove the destructor declaration by deleting its source range
      //Replace the original AST node with the mutated one
      Rewrite.RemoveText(dtorRange);
    }
}
  
void MutatorFrontendAction_288::MutatorASTConsumer_288::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasDefinition(), hasDestructor()).bind("ClassWithVirtualBase");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}