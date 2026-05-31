//header file
#pragma once
#include "Mutator_base.h"

/**
 * misconfigured_template_concept_14
 */ 
class MutatorFrontendAction_14 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(14)

private:
    class MutatorASTConsumer_14 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_14(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misconfigured_template_concept_14.h"

// ========================================================================================================
#define MUT14_OUTPUT 1

void MutatorFrontendAction_14::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto conceptText = stringutils::rangetoStr(*(Result.SourceManager), CD->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      size_t requiresPos = conceptText.find("requires");
      if (requiresPos != std::string::npos) {
          conceptText.insert(requiresPos, "requires ");
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), conceptText);
    }
}
  
void MutatorFrontendAction_14::MutatorASTConsumer_14::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = conceptDecl().bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}