//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Lambda_In_Concept_Requirement_To_Dependent_64
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
        std::vector<const clang::TemplateTypeParmDecl*> templateParams;
    };
};

//source file
#include "../include/Mutate_Lambda_In_Concept_Requirement_To_Dependent_64.h"

// ========================================================================================================
#define MUT64_OUTPUT 1

void MutatorFrontendAction_64::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateParam")) {
      //Filter nodes in header files
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateParams.push_back(TP);
    }
    else if (auto *L = Result.Nodes.getNodeAs<clang::LambdaExpr>("TargetLambda")) {
      //Filter nodes in header files
      if (!L || !Result.Context->getSourceManager().isWrittenInMainFile(
                     L->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaRange = L->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      
      //Find a template parameter to use for dependency
      std::string depParamName;
      if (!templateParams.empty()) {
        size_t idx = getrandom::getRandomIndex(templateParams.size() - 1);
        depParamName = templateParams[idx]->getNameAsString();
      } else {
        // If no template parameter exists, create a dummy one
        depParamName = "T";
      }
      
      //Perform mutation on the source code text by applying string replacement
      // Find the lambda body and insert a requires-clause
      size_t bodyStart = lambdaText.find('{');
      if (bodyStart != std::string::npos) {
        size_t bodyEnd = lambdaText.rfind('}');
        if (bodyEnd != std::string::npos && bodyEnd > bodyStart) {
          std::string newBody = "{ requires A<" + depParamName + ">; }";
          lambdaText.replace(bodyStart, bodyEnd - bodyStart + 1, newBody);
          
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(lambdaRange, lambdaText);
        }
      }
    }
}
  
void MutatorFrontendAction_64::MutatorASTConsumer_64::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    // First matcher to collect template parameters
    DeclarationMatcher paramMatcher = templateTypeParmDecl().bind("TemplateParam");
    // Second matcher to find lambda expressions in concept compound requirements
    StatementMatcher lambdaMatcher = lambdaExpr(
      hasAncestor(
        compoundRequirement(
          hasTypeConstraint(
            hasAncestor(
              requiresExpr()
            )
          )
        )
      )
    ).bind("TargetLambda");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(paramMatcher, &callback);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}