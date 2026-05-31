//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constrained_Variable_Template_Declaration_118
 */ 
class MutatorFrontendAction_118 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(118)

private:
    class MutatorASTConsumer_118 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_118(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Constrained_Variable_Template_Declaration_118.h"

// ========================================================================================================
#define MUT118_OUTPUT 1

void MutatorFrontendAction_118::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("VarTemplateDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      auto *TemplParams = VD->getTemplateParameters();
      if (!TemplParams || TemplParams->size() == 0)
        return;
      
      bool hasRequiresClause = false;
      bool usesConceptDirectly = false;
      std::string conceptName;
      
      // Check template parameters for concept usage
      for (unsigned i = 0; i < TemplParams->size(); ++i) {
        auto *Param = TemplParams->getParam(i);
        if (auto *TTP = dyn_cast<TemplateTypeParmDecl>(Param)) {
          if (TTP->hasTypeConstraint()) {
            usesConceptDirectly = true;
            auto *Constraint = TTP->getTypeConstraint();
            if (Constraint) {
              if (auto *ConceptRef = Constraint->getNamedConcept()) {
                conceptName = ConceptRef->getNameAsString();
              }
            }
          }
        }
      }
      
      // Check for requires clause
      if (VD->getASTContext().getLangOpts().CPlusPlus20) {
        // Check for requires clause by examining the template declaration's constraints
        auto *TemplDecl = VD->getTemplatedDecl();
        if (TemplDecl && TemplDecl->getTrailingRequiresClause()) {
          hasRequiresClause = true;
          // Try to extract concept name from constraints
          if (conceptName.empty()) {
            // For simplicity, we'll use a heuristic to find concept name
            // In practice, this would need more sophisticated AST traversal
            auto SourceRange = VD->getSourceRange();
            if (SourceRange.isValid()) {
              auto SourceText = Lexer::getSourceText(
                  CharSourceRange::getTokenRange(SourceRange),
                  *Result.SourceManager, Result.Context->getLangOpts());
              llvm::StringRef Text = SourceText;
              // Simple heuristic: look for identifier before <
              size_t AnglePos = Text.find('<');
              if (AnglePos != llvm::StringRef::npos) {
                size_t ConceptStart = Text.rfind(' ', AnglePos);
                if (ConceptStart != llvm::StringRef::npos) {
                  conceptName = Text.substr(ConceptStart + 1, AnglePos - ConceptStart - 1).str();
                }
              }
            }
          }
        }
      }
      
      // Only mutate if we have exactly one of the two constraint forms
      if ((hasRequiresClause && !usesConceptDirectly) || (!hasRequiresClause && usesConceptDirectly)) {
        //Get the source code text of target node
        auto originalText = Lexer::getSource2Text(
            CharSourceRange::getTokenRange(VD->getSourceRange()),
            *Result.SourceManager, Result.Context->getLangOpts());
        
        if (conceptName.empty()) {
          return;
        }
        
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedText;
        
        if (hasRequiresClause) {
          // Change from requires clause to concept directly in template parameter
          // Find "template <typename T> requires Concept<T>"
          // Replace with "template <Concept T>"
          std::string originalTextStr = originalText.str();
          size_t templateStart = originalTextStr.find("template");
          if (templateStart == std::string::npos) return;
          
          size_t typenamePos = originalTextStr.find("typename", templateStart);
          if (typenamePos == std::string::npos) {
            typenamePos = originalTextStr.find("class", templateStart);
            if (typenamePos == std::string::npos) return;
          }
          
          size_t paramNameStart = originalTextStr.find_first_not_of(" \t", typenamePos + 8);
          size_t paramNameEnd = originalTextStr.find_first_of(" \t,>", paramNameStart);
          std::string paramName = originalTextStr.substr(paramNameStart, paramNameEnd - paramNameStart);
          
          size_t requiresPos = originalTextStr.find("requires", templateStart);
          if (requiresPos == std::string::npos) return;
          
          // Build new template declaration
          mutatedText = "template <" + conceptName + " " + paramName + "> ";
          mutatedText += originalTextStr.substr(requiresPos + 8); // Skip "requires"
          
          // Remove the concept name from the requires clause part
          size_t conceptPos = mutatedText.find(conceptName + "<" + paramName + ">");
          if (conceptPos != std::string::npos) {
            mutatedText.erase(conceptPos, conceptName.length() + paramName.length() + 2);
          }
        } else if (usesConceptDirectly) {
          // Change from concept directly to requires clause
          // Find "template <Concept T>"
          // Replace with "template <typename T> requires Concept<T>"
          std::string originalTextStr = originalText.str();
          size_t templateStart = originalTextStr.find("template");
          if (templateStart == std::string::npos) return;
          
          size_t ltPos = originalTextStr.find('<', templateStart);
          if (ltPos == std::string::npos) return;
          
          size_t gtPos = originalTextStr.find('>', ltPos);
          if (gtPos == std::string::npos) return;
          
          std::string templateParams = originalTextStr.substr(ltPos + 1, gtPos - ltPos - 1);
          size_t conceptPos = templateParams.find(conceptName);
          if (conceptPos == std::string::npos) return;
          
          size_t paramNameStart = templateParams.find_first_not_of(" \t", conceptPos + conceptName.length());
          std::string paramName = templateParams.substr(paramNameStart);
          
          // Build new template declaration
          mutatedText = "template <typename " + paramName + "> requires " + conceptName + "<" + paramName + "> ";
          mutatedText += originalTextStr.substr(gtPos + 1);
        }
        
        //Replace the original AST node with the mutated one
        if (!mutatedText.empty()) {
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), mutatedText);
        }
      }
    }
}
  
void MutatorFrontendAction_118::MutatorASTConsumer_118::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::varTemplateDecl().bind("VarTemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}