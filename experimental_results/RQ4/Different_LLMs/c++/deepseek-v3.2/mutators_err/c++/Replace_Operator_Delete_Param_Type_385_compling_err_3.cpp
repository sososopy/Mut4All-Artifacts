//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Operator_Delete_Param_Type_385
 */ 
class MutatorFrontendAction_385 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(385)

private:
    class MutatorASTConsumer_385 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_385(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> availableClasses;
    };
};

//source file
#include "../include/Mutator_385.h"

// ========================================================================================================
#define MUT385_OUTPUT 1

void MutatorFrontendAction_385::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      if (!CD->isCompleteDefinition())
        return;
      //Record the node information to be used in the mutation process
      availableClasses.push_back(CD);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("OperatorDelete")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if it's a member operator delete
      if (!FD->isCXXClassMember())
        return;
      if (!FD->isOverloadedOperator())
        return;
      if (FD->getNameAsString() != "operator delete")
        return;
      //Get the source code text of target node
      auto paramTypes = FD->parameters();
      if (paramTypes.empty())
        return;
      //First parameter must be void*
      if (!paramTypes[0]->getType()->isVoidPointerType())
        return;
      //Select a random class from availableClasses
      if (availableClasses.empty())
        return;
      size_t idx = getrandom::getRandomIndex(availableClasses.size() - 1);
      const clang::CXXRecordDecl *targetClass = availableClasses[idx];
      //Perform mutation on the source code text by applying string replacement
      std::string newParamType = targetClass->getNameAsString();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      //Find the first parameter type location
      SourceLocation beginLoc = FD->getBeginLoc();
      SourceRange paramRange = FD->getParamDecl(0)->getSourceRange();
      std::string paramText = stringutils::rangetoStr(*(Result.SourceManager), paramRange);
      //Replace "void*" with the class name
      size_t pos = originalText.find(paramText);
      if (pos == std::string::npos)
        return;
      std::string mutatedText = originalText;
      mutatedText.replace(pos, param2.length(), newParamType);
      mutatedText = "/*mut385*/" + mutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_385::MutatorASTConsumer_385::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("ClassDecl");
    DeclarationMatcher deleteMatcher = functionDecl(hasOverloadedOperatorName("delete")).bind("OperatorDelete");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(deleteMatcher, &callback);
    matchFinder.matchAST(Context);
}