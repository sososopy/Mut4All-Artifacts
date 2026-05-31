//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Explicit_Object_Parameter_Assignment_351
 */ 
class MutatorFrontendAction_351 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(351)

private:
    class MutatorASTConsumer_351 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_351(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Explicit_Object_Parameter_Assignment_351.h"

// ========================================================================================================
#define MUT351_OUTPUT 1

void MutatorFrontendAction_351::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("AssignmentOperator")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if it's a defaulted assignment operator with explicit object parameter
      if (!MT->isDefaulted() || MT->getNumParams() != 2)
        return;
      //Check if first parameter is explicit object parameter of base type
      const ParmVarDecl* firstParam = MT->getParamDecl(0);
      if (!firstParam || !firstParam->isExplicitObjectParameter())
        return;
      //Check if second parameter is const reference of derived type
      const ParmVarDecl* secondParam = MT->getParamDecl(1);
      if (!secondParam || !secondParam->getType().isConstQualified() || !secondParam->getType()->isReferenceType())
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Find the explicit object parameter part
      size_t thisPos = declaration.find("this");
      if (thisPos == std::string::npos)
        return;
      //Find the comma before the second parameter
      size_t commaPos = declaration.find(',', thisPos);
      if (commaPos == std::string::npos)
        return;
      //Remove the explicit object parameter and its type
      size_t startRemove = declaration.find('(', thisPos);
      if (startRemove == std::string::npos)
        return;
      size_t endRemove = commaPos;
      //Perform mutation on the source code text by applying string replacement
      declaration.erase(startRemove, endRemove - startRemove + 1);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_351::MutatorASTConsumer_351::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isDefaulted(), hasName("operator="), hasParameter(0, parmVarDecl(isExplicitObjectParameter())), hasParameter(1, parmVarDecl(hasType(isConstQualifiedReferenceType())))).bind("AssignmentOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}