//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concept_Constraint_In_Constructor_56
 */ 
class MutatorFrontendAction_56 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(56)

private:
    class MutatorASTConsumer_56 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_56(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const ConceptDecl*> conceptDecls;
        std::vector<const FunctionDecl*> functionDecls;
        std::string newConceptName;
    };
};

//source file
#include "../include/Replace_Concept_Constraint_In_Constructor_56.h"

// ========================================================================================================
#define MUT56_OUTPUT 1

void MutatorFrontendAction_56::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("conceptDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      //Record concept declarations
      conceptDecls.push_back(CD);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record function declarations for potential overloads
      functionDecls.push_back(FD);
    }
    else if (auto *Ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constructorDecl")) {
      //Filter nodes in header files
      if (!Ctor || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Ctor->getLocation()))
        return;
      
      // Check if constructor has a requires clause with a concept
      const Expr *RequiresExpr = nullptr;
      if (const auto *Trailing = Ctor->getTrailingRequiresClause()) {
        RequiresExpr = Trailing;
      }
      if (!RequiresExpr) return;
      
      // Find concept usage in requires clause
      bool hasConcept = false;
      std::function<bool(Expr*)> traverse = [&](Expr *E) {
        if (auto *CSE = dyn_cast_or_null<ConceptSpecializationExpr>(E)) {
          hasConcept = true;
          return false;
        }
        for (auto *Child : E->children()) {
          if (auto *ChildExpr = dyn_cast_or_null<Expr>(Child)) {
            if (!traverse(ChildExpr)) return false;
          }
        }
        return true;
      };
      traverse(RequiresExpr);
      if (!hasConcept) return;
      
      // Get the source code text of target node
      auto ctorRange = Ctor->getSourceRange();
      auto ctorText = stringutils::rangetoStr(*(Result.SourceManager), ctorRange);
      
      // Find available concepts for replacement
      if (conceptDecls.size() < 1) return;
      
      std::vector<std::string> availableConcepts;
      for (const auto *CD : conceptDecls) {
        availableConcepts.push_back(CD->getNameAsString());
      }
      
      // Choose a different concept if available
      std::string oldConceptName;
      std::string newConceptName;
      
      // Extract the concept name from requires clause
      size_t requiresPos = ctorText.find("requires");
      if (requiresPos == std::string::npos) return;
      
      // Simple extraction of concept name (for demonstration)
      // In real implementation, would use more robust parsing
      size_t conceptStart = ctorText.find('<', requiresPos);
      if (conceptStart == std::string::npos) return;
      size_t conceptEnd = ctorText.rfind('>', conceptStart);
      if (conceptEnd == std::string::npos || conceptEnd < conceptStart) return;
      
      // Find concept name before template arguments
      size_t nameStart = ctorText.rfind(' ', conceptStart);
      if (nameStart == std::string::npos || nameStart < requiresPos) nameStart = requiresPos + 8;
      oldConceptName = ctorText.substr(nameStart + 1, conceptStart - nameStart - 1);
      
      // Choose a different concept
      for (const auto &concept : availableConcepts) {
        if (concept != oldConceptName) {
          newConceptName = concept;
          break;
        }
      }
      
      // If no other concept exists, create a new one based on function overloads
      if (newConceptName.empty() && !functionDecls.empty()) {
        // Group functions by name
        std::map<std::string, std::vector<const FunctionDecl*>> funcMap;
        for (const auto *FD : functionDecls) {
          funcMap[FD->getNameAsString()].push_back(FD);
        }
        
        // Find a function with multiple overloads
        for (const auto &pair : funcMap) {
          if (pair.second.size() > 1) {
            // Create new concept name
            newConceptName = pair.first + "2";
            
            // Create new concept definition
            std::string newConceptDef = "template<typename T>\nconcept " + newConceptName + " = requires(T t) { " + pair.first + "(t); };\n";
            
            // Insert new concept before the constructor
            SourceLocation insertLoc = Ctor->getBeginLoc();
            Rewrite.InsertTextBefore(insertLoc, "/*mut56*/" + newConceptDef);
            break;
          }
        }
      }
      
      if (newConceptName.empty()) return;
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = ctorText;
      size_t oldPos = mutatedText.find(oldConceptName);
      while (oldPos != std::string::npos) {
        // Check if this is in the requires clause
        size_t requiresBefore = mutatedText.rfind("requires", oldPos);
        if (requiresBefore != std::string::npos && requiresBefore < oldPos) {
          mutatedText.replace(oldPos, oldConceptName.length(), newConceptName);
          break;
        }
        oldPos = mutatedText.find(oldConceptName, oldPos + 1);
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(ctorRange, "/*mut56*/" + mutatedText);
    }
}
  
void MutatorFrontendAction_56::MutatorASTConsumer_56::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher conceptMatcher = cxxRecordDecl().bind("conceptDecl");
    DeclarationMatcher functionMatcher = functionDecl().bind("functionDecl");
    DeclarationMatcher constructorMatcher = cxxConstructorDecl(hasTrailingReturn()).bind("constructorDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(constructorMatcher, &callback);
    matchFinder.matchAST(Context);
}