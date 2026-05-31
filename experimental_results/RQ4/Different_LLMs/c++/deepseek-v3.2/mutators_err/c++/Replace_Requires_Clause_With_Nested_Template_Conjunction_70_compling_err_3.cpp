//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Requires_Clause_With_Nested_Template_Conjunction_70
 */ 
class MutatorFrontendAction_70 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(70)

private:
    class MutatorASTConsumer_70 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_70(Rewriter &R) : TheRewriter(R) {}
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
        std::string holderName;
        bool holderAdded;
    };
};

//source file
#include "../include/Mutator_Replace_Requires_Clause_With_Nested_Template_Conjunction_70.h"

// ========================================================================================================
#define MUT70_OUTPUT 1

void MutatorFrontendAction_70::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTPSD = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("PartialSpec")) {
      //Filter nodes in header files
      if (!CTPSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTPSD->getLocation()))
        return;
      
      // Ensure there is a requires clause
      if (!CTPSD->hasTrailingRequiresClause())
        return;
      
      // Get the source code text of target node
      auto &SM = *Result.SourceManager;
      
      // First, ensure we have a holder struct with a nested template alias
      if (!holderAdded) {
        // Find a suitable struct to add the template alias to, or create a new one
        // For simplicity, we'll add a new struct at the beginning of the file
        // We'll use a unique name to avoid conflicts
        holderName = "Mut70_Holder_" + std::to_string(getrandom::getRandomIndex(1000));
        std::string holderDecl = "struct " + holderName + " { template<template<class> class> using inner_template = int; };\n";
        SourceLocation fileStart = SM.getLocForStartOfFile(SM.getMainFileID());
        Rewrite.InsertText(fileStart, "/*mut70*/" + holderDecl);
        holderAdded = true;
      }
      
      // Get the original requires clause source range
      auto *Trailing = CTPSD->getTrailingRequires();
      if (!Trailing)
        return;
      
      SourceRange requiresRange = Trailing->getSourceRange();
      
      // Build the new requires expression with conjunction
      // We'll keep the original constraints and add a nested template member access
      std::string originalRequires = stringutils::rangetoStr(SM, requiresRange);
      std::string newRequires = "requires requires { typename " + holderName + "::template inner_template<int>; } && ";
      
      // Remove the leading "requires" from original if present
      if (originalRequires.substr(0, 8) == "requires")
        originalRequires = originalRequires.substr(8);
      
      newRequires += originalRequires;
      
      // Perform mutation on the source code text by applying string replacement
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(requiresRange), newRequires);
    }
}
  
void MutatorFrontendAction_70::MutatorASTConsumer_70::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("PartialSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}