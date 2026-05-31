//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Explicit_Object_Parameter_Type_386
 */ 
class MutatorFrontendAction_386 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(386)

private:
    class MutatorASTConsumer_386 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_386(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableTypeNames;
    };
};

//source file
#include "../include/Mutator_Replace_Lambda_Explicit_Object_Parameter_Type_386.h"

// ========================================================================================================
#define MUT386_OUTPUT 1

void MutatorFrontendAction_386::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      
      const LambdaExpr *lambda = MT;
      const CXXRecordDecl *closure = lambda->getLambdaClass();
      if (!closure) return;
      
      // Check if lambda has explicit object parameter
      const FunctionDecl *callOperator = closure->getLambdaCallOperator();
      if (!callOperator) return;
      
      const ParmVarDecl *firstParam = callOperator->getParamDecl(0);
      if (!firstParam) return;
      
      // Verify it's an explicit object parameter (this param)
      // Check if the parameter is a reference to the lambda's closure type
      QualType paramType = firstParam->getType();
      QualType closureType = Result.Context->getRecordType(closure);
      if (!paramType->isReferenceType() || paramType.getNonReferenceType() != closureType) return;
      
      // Get the original type source text
      QualType originalType = firstParam->getType();
      SourceRange typeRange = firstParam->getSourceRange();
      if (typeRange.isInvalid()) return;
      
      // Collect available type names from the context
      ASTContext &Ctx = *Result.Context;
      std::string mutatedTypeName;
      
      // Look for suitable existing types in the same scope
      if (availableTypeNames.empty()) {
        // Collect class/struct/enum names from the translation unit
        auto matcher = cxxRecordDecl().bind("recordDecl");
        MatchFinder finder;
        finder.addMatcher(matcher, this);
        finder.matchAST(Ctx);
        
        // Also collect typedef/using aliases
        auto typeMatcher = typedefDecl().bind("typedefDecl");
        MatchFinder typeFinder;
        typeFinder.addMatcher(typeMatcher, this);
        typeFinder.matchAST(Ctx);
      }
      
      // Choose a type name (prefer existing, otherwise create new)
      if (!availableTypeNames.empty()) {
        size_t idx = getrandom::getRandomIndex(availableTypeNames.size() - 1);
        mutatedTypeName = availableTypeNames[idx];
      } else {
        // No suitable type found, create a forward declaration
        mutatedTypeName = "int";
      }
    }
}