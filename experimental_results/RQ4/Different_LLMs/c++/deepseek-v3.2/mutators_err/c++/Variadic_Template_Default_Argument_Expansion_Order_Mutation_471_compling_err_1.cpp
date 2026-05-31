//header file
#pragma once
#include "Mutator_base.h"

/**
 * Variadic_Template_Default_Argument_Expansion_Order_Mutation_471
 */ 
class MutatorFrontendAction_471 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(471)

private:
    class MutatorASTConsumer_471 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_471(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Variadic_Template_Default_Argument_Expansion_Order_Mutation_471.h"

// ========================================================================================================
#define MUT471_OUTPUT 1

void MutatorFrontendAction_471::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      // Check if it's a template function with variadic parameters
      if (!FD->isTemplate() || FD->getTemplateSpecializationInfo() == nullptr)
        return;
      
      // Get the function template declaration
      const FunctionTemplateDecl *FTD = FD->getDescribedFunctionTemplate();
      if (!FTD) return;
      
      // Check if it has variadic template parameters
      const TemplateParameterList *TPL = FTD->getTemplateParameters();
      if (!TPL || !TPL->containsUnexpandedParameterPack())
        return;
      
      // Check default arguments in parameters
      for (unsigned i = 0; i < FD->getNumParams(); ++i) {
        const ParmVarDecl *Param = FD->getParamDecl(i);
        if (!Param || !Param->hasDefaultArg()) continue;
        
        // Get the default argument expression
        const Expr *DefaultArg = Param->getDefaultArg();
        if (!DefaultArg) continue;
        
        // Get source range of default argument
        SourceRange DefaultArgRange = DefaultArg->getSourceRange();
        if (DefaultArgRange.isInvalid()) continue;
        
        // Get source text of default argument
        std::string DefaultArgText = stringutils::rangetoStr(*(Result.SourceManager), DefaultArgRange);
        
        // Check if it contains a fold expression pattern (right fold)
        size_t rightFoldPos = DefaultArgText.find("||...");
        size_t leftFoldPos = DefaultArgText.find("...||");
        
        // Only mutate if it's a right fold and not already a left fold
        if (rightFoldPos != std::string::npos && leftFoldPos == std::string::npos) {
          // Find the pattern "(expr ||...)" -> change to "(... || expr)"
          size_t openParenPos = DefaultArgText.find('(', rightFoldPos - 10);
          if (openParenPos != std::string::npos) {
            // Extract the expression part before "||..."
            std::string exprPart = DefaultArgText.substr(openParenPos + 1, rightFoldPos - openParenPos - 1);
            
            // Construct left fold version
            std::string mutatedText = "(" + "... || " + exprPart + ")";
            
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(DefaultArgRange), mutatedText);
          }
        }
        // Also check for other binary operators with right fold
        else {
          // Check for other common binary operators in fold expressions
          std::vector<std::string> operators = {"&&", "+", "-", "*", "/", "%", "<", ">", "==", "!=", "<=", ">="};
          for (const std::string &op : operators) {
            std::string rightPattern = op + "...";
            std::string leftPattern = "..." + op;
            size_t rightPos = DefaultArgText.find(rightPattern);
            size_t leftPos = DefaultArgText.find(leftPattern);
            
            if (rightPos != std::string::npos && leftPos == std::string::npos) {
              // Find the opening parenthesis
              size_t openParenPos = DefaultArgText.find('(', rightPos - 10);
              if (openParenPos != std::string::npos) {
                // Extract expression part
                std::string exprPart = DefaultArgText.substr(openParenPos + 1, rightPos - openParenPos - 1);
                
                // Construct left fold version
                std::string mutatedText = "(" + "..." + op + " " + exprPart + ")";
                
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(DefaultArgRange), mutatedText);
                break;
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_471::MutatorASTConsumer_471::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasDefaultArg(anyOf(hasDescendant(binaryOperator()), hasDescendant(unaryExprOrTypeTraitExpr())))).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}