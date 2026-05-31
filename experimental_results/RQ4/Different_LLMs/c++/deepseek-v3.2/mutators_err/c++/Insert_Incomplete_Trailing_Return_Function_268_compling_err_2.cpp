//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Incomplete_Trailing_Return_Function_268
 */ 
class MutatorFrontendAction_268 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(268)

private:
    class MutatorASTConsumer_268 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_268(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> typeNames;
        std::vector<std::string> funcNames;
        std::vector<std::string> varNames;
        bool hasType = false;
        bool hasFuncOrVar = false;
    };
};

//source file
#include "../include/Mutator_268.h"

// ========================================================================================================
#define MUT268_OUTPUT 1

void MutatorFrontendAction_268::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("TypeDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      typeNames.push_back(TD->getNameAsString());
      hasType = true;
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      funcNames.push_back(FD->getNameAsString());
      hasFuncOrVar = true;
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      varNames.push_back(VD->getNameAsString());
      hasFuncOrVar = true;
    }
    else if (auto *TU = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TranslationUnit")) {
      //Filter nodes in header files
      if (!TU || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TU->getLocation()))
        return;
      //Check if mutation conditions are met
      if (!hasType || !hasFuncOrVar) return;
      
      //Select identifiers
      std::string funcIdentifier;
      if (!funcNames.empty()) {
        size_t idx = getrandom::getRandomIndex(funcNames.size() - 1);
        funcIdentifier = funcNames[idx];
      } else {
        size_t idx = getrandom::getRandomIndex(varNames.size() - 1);
        funcIdentifier = varNames[idx];
      }
      
      std::string returnType;
      if (!typeNames.empty()) {
        size_t idx = getrandom::getRandomIndex(typeNames.size() - 1);
        returnType = typeNames[idx];
      } else {
        returnType = "int"; // fallback if no type found
      }
      
      std::string paramType;
      if (!typeNames.empty()) {
        size_t idx = getrandom::getRandomIndex(typeNames.size() - 1);
        paramType = typeNames[idx];
      } else {
        paramType = "int"; // fallback
      }
      
      //Construct incomplete function declaration
      std::string insertion = " auto " + funcIdentifier + "(" + paramType + " -> " + returnType;
      
      //Get the end location of translation unit
      SourceLocation endLoc = TU->getEndLoc();
      //Perform mutation by inserting at end
      Rewrite.InsertTextBefore(endLoc, insertion);
    }
}
  
void MutatorFrontendAction_268::MutatorASTConsumer_268::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher typeMatcher = clang::ast_matchers::typeDecl().bind("TypeDecl");
    DeclarationMatcher funcMatcher = clang::ast_matchers::functionDecl().bind("FunctionDecl");
    DeclarationMatcher varMatcher = clang::ast_matchers::varDecl().bind("VarDecl");
    DeclarationMatcher tuMatcher = clang::ast_matchers::translationUnitDecl().bind("TranslationUnit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(tuMatcher, &callback);
    matchFinder.matchAST(Context);
}