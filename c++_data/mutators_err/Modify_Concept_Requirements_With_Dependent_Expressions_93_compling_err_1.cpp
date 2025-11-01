//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Concept_Requirements_With_Dependent_Expressions_93
 */ 
class MutatorFrontendAction_93 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(93)

private:
    class MutatorASTConsumer_93 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_93(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_concept_requirements_with_dependent_expressions_93.h"

// ========================================================================================================
#define MUT93_OUTPUT 1

void MutatorFrontendAction_93::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CR = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      //Filter nodes in header files
      if (!CR || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CR->getLocation()))
        return;

      //Get the source code text of target node
      auto conceptBody = stringutils::rangetoStr(*(Result.SourceManager), CR->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t pos = conceptBody.find("requires");
      if (pos != std::string::npos) {
          size_t blockStart = conceptBody.find("{", pos);
          size_t blockEnd = conceptBody.find("}", blockStart);
          if (blockStart != std::string::npos && blockEnd != std::string::npos) {
              std::string dependentExpr = "sizeof(t);";
              conceptBody.insert(blockStart + 1, "\n/*mut93*/" + dependentExpr + "\n");
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CR->getSourceRange()), conceptBody);
    }
}
  
void MutatorFrontendAction_93::MutatorASTConsumer_93::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}