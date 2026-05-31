//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Noexcept_Specifier_With_Complex_Expression_11
 */ 
class MutatorFrontendAction_11 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(11)

private:
    class MutatorASTConsumer_11 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_11(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
        std::vector<const clang::FunctionDecl*> targetFunctions;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        
    };
};

//source file
#include "../include/Mutator_Replace_Noexcept_Specifier_With_Complex_Expression_11.h"

// ========================================================================================================
#define MUT11_OUTPUT 1

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TargetFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if function has noexcept specifier
      if (FD->getExceptionSpecType() != clang::EST_Noexcept)
        return;
      //Check if function is a template
      if (!FD->getDescribedFunctionTemplate())
        return;
      //Record candidate function
      targetFunctions.push_back(FD);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("SelectedFunction")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getExceptionSpecType() != clang::EST_Noexcept)
        return;
      if (!FD->getDescribedFunctionTemplate())
        return;