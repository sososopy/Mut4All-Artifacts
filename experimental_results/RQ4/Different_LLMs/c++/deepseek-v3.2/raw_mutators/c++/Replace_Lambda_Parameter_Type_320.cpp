//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Parameter_Type_320
 */ 
class MutatorFrontendAction_320 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(320)

private:
    class MutatorASTConsumer_320 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_320(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> candidateTypes;
    };
};

//source file
#include "../include/Replace_Lambda_Parameter_Type_320.h"

// ========================================================================================================
#define MUT320_OUTPUT 1

void MutatorFrontendAction_320::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      const FunctionDecl* enclosingFunc = nullptr;
      if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("EnclosingFunc")) {
        enclosingFunc = FD;
      }

      // Collect candidate integral/floating types from context
      candidateTypes.clear();
      if (enclosingFunc) {
        // Collect from function parameters
        for (const ParmVarDecl* param : enclosingFunc->parameters()) {
          QualType paramType = param->getType();
          if (paramType->isIntegerType() || paramType->isFloatingType()) {
            candidateTypes.push_back(paramType.getAsString());
          }
        }
        // Collect from local variables in function body
        if (enclosingFunc->hasBody()) {
          Stmt* body = enclosingFunc->getBody();
          if (body) {
            // Traverse body to find VarDecls (simplified approach)
            // For simplicity, we'll rely on existing types in the context
          }
        }
      }

      // Also consider global types visible in the translation unit
      // This is simplified; in practice we might need to traverse more context

      // If no candidate types found, add fundamental types
      if (candidateTypes.empty()) {
        candidateTypes.push_back("int");
        candidateTypes.push_back("double");
        candidateTypes.push_back("float");
        candidateTypes.push_back("long");
        candidateTypes.push_back("short");
        candidateTypes.push_back("char");
      }

      // Get lambda's call operator (if available) to examine parameters
      const CXXMethodDecl* callOperator = MT->getCallOperator();
      if (!callOperator) return;

      // Check if lambda has explicit parameter types
      const ParmVarDecl* firstParam = nullptr;
      for (const ParmVarDecl* param : callOperator->parameters()) {
        if (param->hasExplicitParamType()) {
          firstParam = param;
          break;
        }
      }

      if (!firstParam) return; // No explicitly typed parameter to mutate

      // Get original parameter type
      QualType originalType = firstParam->getType();
      std::string originalTypeStr = originalType.getAsString();

      // Select replacement type (different from original)
      std::string replacementType;
      for (const std::string& candidate : candidateTypes) {
        if (candidate != originalTypeStr) {
          replacementType = candidate;
          break;
        }
      }
      if (replacementType.empty()) {
        // Fallback: use first candidate if all are same as original (unlikely)
        replacementType = candidateTypes[0];
      }

      // Get source range of the parameter type
      SourceRange paramTypeRange = firstParam->getSourceRange();
      if (!paramTypeRange.isValid()) return;

      // Perform mutation on the source code text by applying string replacement
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(paramTypeRange, replacementType);
    }
}
  
void MutatorFrontendAction_320::MutatorASTConsumer_320::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(hasParent(functionDecl().bind("EnclosingFunc"))).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}