//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Decl.h"

/**
 * Remove_Throw_Specifier_11
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
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Remove_Throw_Specifier_11.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

// ========================================================================================================
#define MUT11_OUTPUT 1

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = FD->getLocStart().printToString(Result.Context->getSourceManager());
      //Perform mutation on the source code text by applying string replacement
      if (FD->hasExceptionSpec()) {
        auto exceptionSpec = FD->getExceptionSpecType();
        if (exceptionSpec == clang::FunctionDecl::ExceptionSpecType::EST_DynamicNone) {
          declaration = declaration.substr(0, declaration.find("throw")) + declaration.substr(declaration.find("throw") + 6);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_11::MutatorASTConsumer_11::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasExceptionSpec()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}