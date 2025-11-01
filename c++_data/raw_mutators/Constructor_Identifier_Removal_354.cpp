//header file
#pragma once
#include "Mutator_base.h"

/**
 * Constructor_Identifier_Removal_354
 */ 
class MutatorFrontendAction_354 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(354)

private:
    class MutatorASTConsumer_354 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_354(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Constructor_Identifier_Removal_354.h"

// ========================================================================================================
#define MUT354_OUTPUT 1

void MutatorFrontendAction_354::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!Ctor || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Ctor->getLocation()))
        return;

      // Get the source code text of target node
      auto constructorRange = Ctor->getSourceRange();
      auto constructorText = stringutils::rangetoStr(*(Result.SourceManager), constructorRange);

      // Perform mutation on the source code text by applying string replacement
      // Removing any extraneous identifiers before the constructor name
      std::string className = Ctor->getParent()->getNameAsString();
      size_t pos = constructorText.find(className);
      if (pos != std::string::npos) {
        constructorText = constructorText.substr(pos);
      }

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(constructorRange), constructorText);
    }
}
  
void MutatorFrontendAction_354::MutatorASTConsumer_354::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}