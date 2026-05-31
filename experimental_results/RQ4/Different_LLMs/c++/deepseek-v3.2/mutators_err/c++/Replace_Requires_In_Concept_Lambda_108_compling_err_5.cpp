//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Requires_In_Concept_Lambda_108
 */ 
class MutatorFrontendAction_108 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(108)

private:
    class MutatorASTConsumer_108 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_108(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableConcepts;
        std::vector<std::string> templateParamNames;
    };
};

//source file
#include "../include/Mutator_Replace_Requires_In_Concept_Lambda_108.h"

// ========================================================================================================
#define MUT108_OUTPUT 1

void MutatorFrontendAction_108::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      
      // Check if this lambda is inside a concept definition
      auto* parent = Result.Context->getParents(*MT)[0].get<clang::Decl>();
      if (!parent || !llvm::isa<clang::ConceptDecl>(parent))
        return;
      
      // Get the lambda body
      auto* body = MT->getBody();
      if (!body) return;
      
      // Find requires expressions in the lambda body
      std::vector<const clang::RequiresExpr*> requiresExprs;
      // Use a custom visitor to collect requires expressions
      struct RequiresExprVisitor : public clang::RecursiveASTVisitor<RequiresExprVisitor> {
        std::vector<const clang::RequiresExpr*> requiresExprs;
        bool VisitRequiresExpr(clang::RequiresExpr *RE) {
          requiresExprs.push_back(RE);
          return true;
        }
      } visitor;
      visitor.TraverseStmt(body);
      requiresExprs = visitor.requiresExprs;
      
      // Collect available concepts from the translation unit
      if (availableConcepts.empty()) {
        auto decls = Result.Context->getTranslationUnitDecl()->decls();
        for (auto* decl : decls) {
          if (auto* conceptDecl = llvm::dyn_cast<clang::ConceptDecl>(decl)) {
            availableConcepts.push_back(conceptDecl->getNameAsString());
          }
        }
      }
      
      // Collect template parameter names from the concept
      auto* conceptDecl = llvm::dyn_cast<clang::ConceptDecl>(parent);
      if (conceptDecl && templateParamNames.empty()) {
        auto* tplParams = conceptDecl->getTemplateParameters();
        if (tplParams) {
          for (unsigned i = 0; i < tplParams->size(); ++i) {
            auto* param = tplParams->getParam(i);
            if (param->getIdentifier()) {
              templateParamNames.push_back(param->getNameAsString());
            }
          }
        }
      }
      
      // Get the source code text of target node
      auto lambdaRange = MT->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      
      // Find requires expressions in the lambda text
      size_t requiresPos = lambdaText.find("requires");
      if (requiresPos == std::string::npos) return;
      
      // Find the matching requires expression end
      size_t braceCount = 0;
      size_t endPos = requiresPos;
      bool inBraces = false;
      
      for (size_t i = requiresPos; i < lambdaText.size(); ++i) {
        if (lambdaText[i] == '{') {
          braceCount++;
          inBraces = true;
        } else if (lambdaText[i] == '}') {
          braceCount--;
          if (inBraces && braceCount == 0) {
            endPos = i;
            break;
          }
        }
      }
      
      if (endPos == requiresPos) return;
      
      std::string requiresExpr = lambdaText.substr(requiresPos, endPos - requiresPos + 1);
      
      // Parse the requires expression to get parameters
      size_t paramStart = requiresExpr.find('(');
      size_t paramEnd = requiresExpr.find(')');
      if (paramStart == std::string::npos || paramEnd == std::string::npos) return;
      
      std::string params = requiresExpr.substr(paramStart + 1, paramEnd - paramStart - 1);
      
      // Determine new parameters
      std::string newParams = params;
      if (!params.empty()) {
        newParams += ", auto... args";
      } else if (!templateParamNames.empty()) {
        newParams = templateParamNames[0] + " t, auto... args";
      } else {
        newParams = "auto t, auto... args";
      }
      
      // Determine new concept to use
      std::string newConcept;
      if (!availableConcepts.empty()) {
        // Filter out the current concept if it's in the list
        std::vector<std::string> filteredConcepts;
        for (const auto& concept : availableConcepts) {
          if (concept != conceptDecl->getNameAsString()) {
            filteredConcepts.push_back(concept);
          }
        }
        
        if (!filteredConcepts.empty()) {
          size_t idx = getrandom::getRandomIndex(filteredConcepts.size() - 1);
          newConcept = filteredConcepts[idx];
        } else {
          newConcept = "std::same_as<" + (templateParamNames.empty() ? "T" : templateParamNames[0]) + ">";
        }
      } else {
        newConcept = "std::same_as<" + (templateParamNames.empty() ? "T" : templateParamNames[0]) + ">";
      }
      
      // Find the constraint inside the requires expression
      size_t constraintStart = requiresExpr.find('{', paramEnd);
      if (constraintStart == std::string::npos) return;
      
      size_t constraintEnd = requiresExpr.rfind('}');
      if (constraintEnd == std::string::npos) return;
      
      std::string constraint = requiresExpr.substr(constraintStart, constraintEnd - constraintStart + 1);
      
      // Modify the constraint to use the new concept
      size_t arrowPos = constraint.find("->");
      if (arrowPos != std::string::npos) {
        size_t semicolonPos = constraint.find(';', arrowPos);
        if (semicolonPos != std::string::npos) {
          std::string newConstraint = constraint.substr(0, arrowPos + 2) + " " + newConcept + constraint.substr(semicolonPos);
          
          // Build new requires expression
          std::string newRequiresExpr = "requires(" + newParams + ") " + newConstraint;
          
          // Replace in the lambda text
          lambdaText.replace(requiresPos, endPos - requiresPos + 1, newRequiresExpr);
          
          // Add mutation marker
          lambdaText.insert(requiresPos, "/*mut108*/");
          
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), lambdaText);
        }
      }
    }
}
  
void MutatorFrontendAction_108::MutatorASTConsumer_108::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(hasAncestor(recordDecl(hasAttr(clang::attr::CXX11))))).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}