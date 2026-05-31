//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Dependent_Conversion_To_Explicit_Cast_160
 */ 
class MutatorFrontendAction_160 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(160)

private:
    class MutatorASTConsumer_160 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_160(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXConversionDecl*> conversionTemplates;
    };
};

//source file
#include "../include/Mutator_Replace_Dependent_Conversion_To_Explicit_Cast_160.h"

// ========================================================================================================
#define MUT160_OUTPUT 1

void MutatorFrontendAction_160::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConversionDecl>("conversionDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      if (CD->isTemplateDecl()) {
        conversionTemplates.push_back(CD);
      }
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CXXDependentScopeMemberExpr>("depMemberExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getLocation()))
        return;
      //Get the source code text of target node
      auto memberName = CE->getMember().getAsString();
      if (!memberName.startswith("operator ")) return;
      
      // Check if this matches any recorded conversion template
      bool isConversionTemplateCall = false;
      for (auto convDecl : conversionTemplates) {
        if (convDecl->getParent()->getQualifiedNameAsString() == 
            CE->getBase()->getType().getAsString()) {
          isConversionTemplateCall = true;
          break;
        }
      }
      if (!isConversionTemplateCall) return;
      
      // Extract the target type from "operator Type"
      std::string opStr = memberName.str();
      std::string targetType = opStr.substr(9); // Remove "operator "
      // Remove trailing parentheses if present
      if (targetType.back() == '(') {
        targetType.pop_back();
      }
      
      // Look for an integer literal in the surrounding context
      std::string replacementValue = "0";
      if (auto parent = dyn_cast<clang::BinaryOperator>(CE->getParent())) {
        if (parent->getOpcode() == BO_Comma) {
          if (auto sibling = dyn_cast<clang::IntegerLiteral>(parent->getLHS())) {
            replacementValue = stringutils::rangetoStr(*(Result.SourceManager), 
                                                      sibling->getSourceRange());
          }
        }
      }
      
      //(optional)Record the node information to be used in the mutation process
      //Perform mutation on the source code text by applying string replacement
      int choice = getrandom::getRandomIndex(1);
      std::string newExpr;
      if (choice == 0) {
        newExpr = "(" + targetType + ")" + replacementValue;
      } else {
        newExpr = targetType + "(" + replacementValue + ")";
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CE->getSourceRange(), newExpr);
    }
}
  
void MutatorFrontendAction_160::MutatorASTConsumer_160::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher convMatcher = cxxConversionDecl(isTemplateDecl()).bind("conversionDecl");
    StatementMatcher exprMatcher = cxxDependentScopeMemberExpr(
        hasMemberNameStartingWith("operator "),
        unless(isExpansionInSystemHeader())
    ).bind("depMemberExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(convMatcher, &callback);
    matchFinder.addMatcher(exprMatcher, &callback);
    matchFinder.matchAST(Context);
}