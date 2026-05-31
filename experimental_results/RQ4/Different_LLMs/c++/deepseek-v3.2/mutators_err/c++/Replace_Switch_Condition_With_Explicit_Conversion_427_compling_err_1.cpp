//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Switch_Condition_With_Explicit_Conversion_427
 */ 
class MutatorFrontendAction_427 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(427)

private:
    class MutatorASTConsumer_427 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_427(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> classesWithConversion;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT427_OUTPUT 1

void MutatorFrontendAction_427::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      const Expr* condition = MT->getCond();
      if (!condition)
        return;
      
      QualType condType = condition->getType();
      if (condType->isIntegralOrEnumerationType())
        return;
      
      const CXXRecordDecl* classDecl = nullptr;
      if (condType->isRecordType()) {
        classDecl = condType->getAsCXXRecordDecl();
      }
      
      if (!classDecl)
        return;
      
      // Check for conversion operator to enum type
      bool hasEnumConversion = false;
      QualType targetEnumType;
      for (auto method : classDecl->methods()) {
        if (method->isConversionFunction()) {
          QualType convType = method->getConversionType();
          if (convType->isEnumeralType()) {
            hasEnumConversion = true;
            targetEnumType = convType;
            break;
          }
        }
      }
      
      if (!hasEnumConversion)
        return;
      
      //Get the source code text of target node
      auto condRange = CharSourceRange::getTokenRange(condition->getSourceRange());
      std::string condText = stringutils::rangetoStr(*Result.SourceManager, condRange);
      
      //Perform mutation on the source code text by applying string replacement
      std::string enumTypeName = targetEnumType.getAsString();
      std::string mutatedCond = "static_cast<" + enumTypeName + ">(" + condText + ")";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(condRange, mutatedCond);
    }
    else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      
      // Check if this class has conversion to enum
      for (auto method : CD->methods()) {
        if (method->isConversionFunction()) {
          QualType convType = method->getConversionType();
          if (convType->isEnumeralType()) {
            classesWithConversion.push_back(CD);
            break;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_427::MutatorASTConsumer_427::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher switchMatcher = switchStmt().bind("SwitchStmt");
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(switchMatcher, &callback);
    matchFinder.addMatcher(classMat1cher, &callback);
    matchFinder.matchAST(Context);
}