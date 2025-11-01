//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Using_Declaration_327
 */ 
class MutatorFrontendAction_327 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(327)

private:
    class MutatorASTConsumer_327 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_327(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Incomplete_Using_Declaration_327.h"

// ========================================================================================================
#define MUT327_OUTPUT 1

void MutatorFrontendAction_327::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UD = Result.Nodes.getNodeAs<clang::UsingDecl>("UsingDecl")) {
      //Filter nodes in header files
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto usingText = stringutils::rangetoStr(*(Result.SourceManager), UD->getSourceRange());
      auto pos = usingText.find("::");
      if (pos != std::string::npos) {
        //Perform mutation on the source code text by applying string replacement
        usingText = usingText.substr(0, pos + 2); // Remove everything after "::"
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(UD->getSourceRange()), usingText);
      }
    }
}
  
void MutatorFrontendAction_327::MutatorASTConsumer_327::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = usingDecl(hasAncestor(cxxRecordDecl(isDerivedFrom(cxxRecordDecl())))).bind("UsingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}