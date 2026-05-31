//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Lambda_Constraint_In_Concept_64
 */ 
class MutatorFrontendAction_64 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(64)
    
    private:
      class MutatorASTConsumer_64 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_64(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_lambda_constraint_in_concept_64"

// ========================================================================================================
#define MUT64_OUTPUT 1

void MutatorFrontendAction_64::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto lambda_ins = "->M<[]<A>(){}>";
      if (content.rfind('}') != string::npos)
        content.insert(content.rfind('}'), lambda_ins);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
  }

void MutatorFrontendAction_64::MutatorASTConsumer_64::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}