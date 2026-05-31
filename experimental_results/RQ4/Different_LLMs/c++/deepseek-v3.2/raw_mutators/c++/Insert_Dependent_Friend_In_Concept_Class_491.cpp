//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Dependent_Friend_In_Concept_Class_491
 */ 
class MutatorFrontendAction_491 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(491)

private:
    class MutatorASTConsumer_491 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_491(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> usedParamNames;
    };
};

//source file
#include "../include/Mutator_491.h"

// ========================================================================================================
#define MUT491_OUTPUT 1

void MutatorFrontendAction_491::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ConceptClass")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      //Get the source code text of target node
      auto *TemplateParams = CTD->getTemplateParameters();
      if (!TemplateParams || TemplateParams->size() != 1)
        return;
      
      auto *Param = TemplateParams->getParam(0);
      if (!Param)
        return;
      
      // Check if the parameter is constrained by a concept
      bool isConceptConstrained = false;
      std::string ConceptName;
      if (auto *TTP = dyn_cast<TemplateTypeParmDecl>(Param)) {
        if (TTP->hasTypeConstraint()) {
          isConceptConstrained = true;
          auto *Constraint = TTP->getTypeConstraint();
          if (Constraint) {
            ConceptName = Constraint->getConstraintExpr()->getSourceRange().getAsString();
          }
        }
      }
      
      if (!isConceptConstrained)
        return;
      
      // Record existing parameter names to avoid conflicts
      std::string OuterParamName = Param->getNameAsString();
      usedParamNames.push_back(OuterParamName);
      
      // Generate a new parameter name distinct from existing ones
      std::string NewParamName = "V";
      while (std::find(usedParamNames.begin(), usedParamNames.end(), NewParamName) != usedParamNames.end()) {
        NewParamName += "_";
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string FriendDecl = "\n    template <" + ConceptName + " " + NewParamName + "> friend class " + CTD->getNameAsString() + ";";
      
      //Find the class body end location
      auto *CDecl = CTD->getTemplatedDecl();
      if (!CDecl)
        return;
      
      SourceLocation InsertLoc = CDecl->getEndLoc();
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextBefore(InsertLoc, FriendDecl);
    }
}
  
void MutatorFrontendAction_491::MutatorASTConsumer_491::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("ConceptClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}