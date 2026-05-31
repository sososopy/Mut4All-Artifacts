//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Concept_Constraint_For_NTTP_With_Default_129
 */ 
class MutatorFrontendAction_129 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(129)

private:
    class MutatorASTConsumer_129 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_129(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ConceptDecl*> conceptDecls;
    };
};

//source file
#include "../include/Mutator_129.h"

// ========================================================================================================
#define MUT129_OUTPUT 1

void MutatorFrontendAction_129::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("conceptDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      conceptDecls.push_back(CD);
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("templateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Get the source code text of target node
      auto *TemplDecl = dyn_cast<clang::ClassTemplateDecl>(TD);
      if (!TemplDecl) return;
      auto *Templ = TemplDecl->getTemplateParameters();
      if (!Templ) return;
      
      bool foundTarget = false;
      const clang::ConceptDecl* targetConcept = nullptr;
      
      for (unsigned i = 0; i < Templ->size(); ++i) {
        auto *Param = Templ->getParam(i);
        if (auto *NTTP = dyn_cast<clang::NonTypeTemplateParmDecl>(Param)) {
          if (NTTP->hasDefaultArgument()) {
            // Use getPlaceholderTypeConstraint for older Clang versions
            if (auto *TypeConstraint = NTTP->getPlaceholderTypeConstraint()) {
              if (auto *ConceptRef = dyn_cast<clang::ConceptReference>(TypeConstraint)) {
                targetConcept = ConceptRef->getNamedConcept();
                foundTarget = true;
                break;
              }
            }
          }
        }
      }
      
      if (!foundTarget || !targetConcept) return;
      
      //Find the matching concept declaration from recorded concepts
      const clang::ConceptDecl* conceptToMutate = nullptr;
      for (auto* CD : conceptDecls) {
        if (CD == targetConcept) {
          conceptToMutate = CD;
          break;
        }
      }
      
      if (!conceptToMutate) return;
      
      //Get the constraint expression
      auto* ConstraintExpr = conceptToMutate->getConstraintExpr();
      if (!ConstraintExpr) return;
      
      SourceRange ExprRange = ConstraintExpr->getSourceRange();
      if (ExprRange.isInvalid()) return;
      
      //Generate a new constraint expression
      std::string newConstraint;
      int choice = getrandom::getRandomIndex(3);
      
      switch (choice) {
        case 0:
          newConstraint = "sizeof(int) > 0";
          break;
        case 1:
          newConstraint = "sizeof(char) == 1";
          break;
        case 2:
          newConstraint = "true";
          break;
        case 3:
          newConstraint = "false";
          break;
        default:
          newConstraint = "true";
      }
      
      //Check if concept has template parameters we can use
      if (auto* TemplParams = conceptToMutate->getTemplateParameters()) {
        if (TemplParams->size() > 0) {
          int useTemplateParam = getrandom::getRandomIndex(1);
          if (useTemplateParam) {
            auto* FirstParam = TemplParams->getParam(0);
            if (auto* TypeParam = dyn_cast<clang::TemplateTypeParmDecl>(FirstParam)) {
              std::string paramName = TypeParam->getNameAsString();
              if (!paramName.empty()) {
                newConstraint = "sizeof(" + paramName + ") >= 0";
              }
            }
          }
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = "/*mut129*/" + newConstraint;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(ExprRange, mutatedText);
    }
}
  
void MutatorFrontendAction_129::MutatorASTConsumer_129::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher conceptMatcher = cxxConceptDecl().bind("conceptDecl");
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("templateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}