//header file
#pragma once
#include "Mutator_base.h"

/**
 * Conditional_Operator_Arrays_Different_Sizes_Same_Element_Type_498
 */ 
class MutatorFrontendAction_498 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(498)

private:
    class MutatorASTConsumer_498 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_498(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> arrayDecls;
        std::vector<std::string> arrayNames;
        std::vector<std::string> arrayElementTypes;
        std::vector<int> arraySizes;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT498_OUTPUT 1

void MutatorFrontendAction_498::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record array declarations for later use
      if (VD->getType()->isArrayType()) {
        arrayDecls.push_back(VD);
        arrayNames.push_back(VD->getNameAsString());
        QualType elementType = VD->getType()->getAsArrayTypeUnsafe()->getElementType();
        arrayElementTypes.push_back(elementType.getAsString());
        if (const ConstantArrayType *CAT = dyn_cast<ConstantArrayType>(VD->getType()->getAsArrayTypeUnsafe())) {
          arraySizes.push_back(CAT->getSize().getZExtValue());
        } else {
          arraySizes.push_back(-1); // Variable-sized array
        }
      }
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::ConditionalOperator>("condOp")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getLocation()))
        return;
      //Get the source code text of target node
      Expr* trueExpr = CE->getTrueExpr();
      Expr* falseExpr = CE->getFalseExpr();
      QualType trueType = trueExpr->getType();
      QualType falseType = falseExpr->getType();
      
      //Check if either operand is an array type
      bool trueIsArray = trueType->isArrayType();
      bool falseIsArray = falseType->isArrayType();
      
      if (!trueIsArray && !falseIsArray) return;
      
      //Determine which operand to mutate
      Expr* targetExpr = nullptr;
      QualType targetType;
      bool mutateTrue = false;
      
      if (trueIsArray) {
        targetExpr = trueExpr;
        targetType = trueType;
        mutateTrue = true;
      } else {
        targetExpr = falseExpr;
        targetType = falseType;
        mutateTrue = false;
      }
      
      //Get element type of target array
      QualType targetElementType = targetType->getAsArrayTypeUnsafe()->getElementType();
      std::string targetElementTypeStr = targetElementType.getAsString();
      
      //Find arrays with same element type but different size
      std::vector<int> candidateIndices;
      for (size_t i = 0; i < arrayDecls.size(); ++i) {
        if (arrayElementTypes[i] == targetElementTypeStr) {
          //Check if size is different
          int targetSize = -1;
          if (const ConstantArrayType *CAT = dyn_cast<ConstantArrayType>(targetType->getAsArrayTypeUnsafe())) {
            targetSize = CAT->getSize().getZExtValue();
          }
          if (arraySizes[i] != targetSize && arraySizes[i] != -1) {
            candidateIndices.push_back(i);
          }
        }
      }
      
      if (candidateIndices.empty()) {
        //No suitable array found, create a new declaration
        int newSize = 10; // Default new size
        if (const ConstantArrayType *CAT = dyn_cast<ConstantArrayType>(targetType->getAsArrayTypeUnsafe())) {
          int originalSize = CAT->getSize().getZExtValue();
          newSize = originalSize * 2; // Double the size
        }
        
        //Create new array declaration
        std::string newArrayName = "mut498_arr";
        std::string newDecl = targetElementTypeStr + " " + newArrayName + "[" + std::to_string(newSize) + "];\n";
        
        //Insert declaration before the conditional operator
        SourceLocation insertLoc = CE->getBeginLoc();
        Rewrite.InsertTextBefore(insertLoc, "/*mut498*/" + newDecl);
        
        //Replace the array operand with new array
        if (mutateTrue) {
          Rewrite.ReplaceText(CE->getTrueExpr()->getSourceRange(), newArrayName);
        } else {
          Rewrite.ReplaceText(CE->getFalseExpr()->getSourceRange(), newArrayName);
        }
      } else {
        //Use existing array with different size
        size_t chosenIndex = getrandom::getRandomIndex(candidateIndices.size() - 1);
        std::string replacementArray = arrayNames[candidateIndices[chosenIndex]];
        
        if (mutateTrue) {
          Rewrite.ReplaceText(CE->getTrueExpr()->getSourceRange(), replacementArray);
        } else {
          Rewrite.ReplaceText(CE->getFalseExpr()->getSourceRange(), replacementArray);
        }
      }
    }
}
  
void MutatorFrontendAction_498::MutatorASTConsumer_498::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher arrayMatcher = varDecl().bind("arrayDecl");
    StatementMatcher condMatcher = conditionalOperator().bind("condOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(arrayMatcher, &callback);
    matchFinder.addMatcher(condMatcher, &callback);
    matchFinder.matchAST(Context);
}