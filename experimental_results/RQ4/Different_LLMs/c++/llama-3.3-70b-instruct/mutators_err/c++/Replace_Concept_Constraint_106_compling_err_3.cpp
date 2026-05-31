//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Tooling/Tooling.h"
#include <clang/Rewrite/Core/Rewriter.h>

/**
 * Replace_Concept_Constraint_106
 */ 
class MutatorFrontendAction_106 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(106)

private:
    class MutatorASTConsumer_106 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_106(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Concept_Constraint_106.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

// ========================================================================================================
#define MUT106_OUTPUT 1

void MutatorFrontendAction_106::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateParameterList>(("TemplateParameterList"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      // Identify the concept constraint to replace
      size_t pos = declaration.find("IsFoo");
      if (pos != std::string::npos) {
        // Replace the concept constraint with a different one
        declaration.replace(pos, 5, "IsBar");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_106::MutatorASTConsumer_106::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxTemplateParameterList().bind("TemplateParameterList");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}