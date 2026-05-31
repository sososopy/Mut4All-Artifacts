//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Class_Template_Partial_Specialization_With_Concept_To_Primary_Template_50
 */ 
class MutatorFrontendAction_50 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(50)
    
    private:
      class MutatorASTConsumer_50 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_50(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<std::string> concept_names;
      };
    };

