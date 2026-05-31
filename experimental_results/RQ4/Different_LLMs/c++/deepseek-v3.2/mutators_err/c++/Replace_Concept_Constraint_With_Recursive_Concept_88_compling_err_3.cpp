//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concept_Constraint_With_Recursive_Concept_88
 */ 
class MutatorFrontendAction_88 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(88)

private:
    class MutatorASTConsumer_88 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_88(Rewriter &R) : TheRewriter(R) {}
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
        std::map<std::string, std::string> conceptReplacements;
    };
};

//source file
#include "../include/Mutator_Replace_Concept_Constraint_With_Recursive_Concept_88.h"

// ========================================================================================================
#define MUT88_OUTPUT 1

void MutatorFrontendAction_88::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateTypeParm")) {
      //Filter nodes in header files
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      
      // Check if this template parameter has a concept constraint
      if (!TP->hasTypeConstraint())
        return;
      
      const auto *Constraint = TP->getTypeConstraint();
      if (!Constraint)
        return;
      
      // Get the concept name
      std::string ConceptName;
      const Expr *ConstraintExpr = Constraint->getImmediatelyDeclaredConstraint();
      if (ConstraintExpr) {
        if (const auto *ConceptRef = dyn_cast_or_null<clang::ConceptSpecializationExpr>(ConstraintExpr)) {
          if (const auto *ConceptDecl = ConceptRef->getNamedConcept()) {
            ConceptName = ConceptDecl->getNameAsString();
          }
        }
      }
      if (ConceptName.empty())
        return;
      
      // Check if we already created a recursive concept for this concept
      std::string NewConceptName;
      auto it = conceptReplacements.find(ConceptName);
      if (it != conceptReplacements.end()) {
        NewConceptName = it->second;
      } else {
        // Create a new recursive concept name
        NewConceptName = "Recursive_" + ConceptName;
        conceptReplacements[ConceptName] = NewConceptName;
        
        // Insert the new concept definition before the template
        std::string NewConceptDef = "\ntemplate<typename U>\nconcept " + NewConceptName + " = " + 
                                   ConceptName + "<U> && " + NewConceptName + "<U>;\n";
        
        // Find a good insertion point (before the template that uses this concept)
        SourceLocation InsertLoc = TP->getBeginLoc();
        // Try to find the start of the translation unit or a suitable location
        if (auto *Parent = dyn_cast<Decl>(TP->getDeclContext())) {
          InsertLoc = Parent->getBeginLoc();
        }
        
        // Insert the new concept definition
        Rewrite.InsertTextBefore(InsertLoc, "/*mut88*/" + NewConceptDef);
      }
      
      // Get the source range of the concept name in the template parameter
      SourceRange ConceptRange;
      const Expr *ConstraintExpr2 = Constraint->getImmediatelyDeclaredConstraint();
      if (ConstraintExpr2) {
        if (const auto *ConceptRef = dyn_cast_or_null<clang::ConceptSpecializationExpr>(ConstraintExpr2)) {
          ConceptRange = ConceptRef->getSourceRange();
        } else {
          // Fallback: try to get the range from the constraint
          ConceptRange = Constraint->getSourceRange();
        }
      }
      
      if (ConceptRange.isValid()) {
        //Replace the original concept name with the new recursive concept name
        Rewrite.ReplaceText(ConceptRange, NewConceptName);
      }
    }
}
  
void MutatorFrontendAction_88::MutatorASTConsumer_88::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTypeParmDecl().bind("TemplateTypeParm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}