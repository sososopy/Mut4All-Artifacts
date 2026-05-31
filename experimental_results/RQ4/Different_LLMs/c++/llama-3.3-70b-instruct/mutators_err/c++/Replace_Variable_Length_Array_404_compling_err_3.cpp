//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Variable_Length_Array_404
 */ 
class MutatorFrontendAction_404 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(404)

private:
    class MutatorASTConsumer_404 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_404(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Variable_Length_Array_404.h"

// ========================================================================================================
#define MUT404_OUTPUT 1

void MutatorFrontendAction_404::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>(("VarDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Check if the variable is a variable length array
      if (MT->getType()->isArrayType()) {
        const auto *arrayType = MT->getType()->getAsArrayTypeUnsafe();
        if (arrayType->getSize() == nullptr) {
          //Replace the variable length array with a fixed-length array
          std::string replacement = declaration;
          replacement.replace(replacement.find("[]"), 2, "[256]");
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), replacement);
        }
      }
    }
    if (auto *MT = Result.Nodes.getNodeAs<clang::ParmVarDecl>(("ParmVarDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Check if the parameter is a variable length array
      if (MT->getType()->isArrayType()) {
        const auto *arrayType = MT->getType()->getAsArrayTypeUnsafe();
        if (arrayType->getSize() == nullptr) {
          //Replace the variable length array with a pointer type
          std::string replacement = declaration;
          replacement.replace(replacement.find("[]"), 2, "*");
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), replacement);
        }
      }
    }
}
  
void MutatorFrontendAction_404::MutatorASTConsumer_404::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = varDecl(hasType(arrayType(hasSize(-1)))).bind("VarDecl");
    DeclarationMatcher matcher2 = parmVarDecl(hasType(arrayType(hasSize(-1)))).bind("ParmVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}