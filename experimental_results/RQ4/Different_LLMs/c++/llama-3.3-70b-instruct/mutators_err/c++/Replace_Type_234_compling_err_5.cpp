//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * Replace_Type_234
 */ 
class MutatorFrontendAction_234 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(234)

private:
    class MutatorASTConsumer_234 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_234(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::vector<const clang::Type*> existingTypes;
    };
};

//source file
#include "../include/Replace_Type_234.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

// ========================================================================================================
#define MUT234_OUTPUT 1

void MutatorFrontendAction_234::MutatorASTConsumer_234::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varMatcher = varDecl().bind("VarDecls");
    DeclarationMatcher parmMatcher = parmVarDecl().bind("ParmDecls");
    DeclarationMatcher typeMatcher = typeDecl(hasName()).bind("TypeDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(parmMatcher, &callback);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.matchAST(Context);
}

void MutatorFrontendAction_234::Callback::run(const MatchFinder::MatchResult &Result) {
      //Check whether the matched AST node is the target node
      if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto type = VD->getType();
      if (!type)
        return;
      //Perform mutation on the source code text by applying string replacement
      if (existingTypes.size() > 0) {
        int choice = rand() % existingTypes.size();
        auto newType = existingTypes[choice];
        if (newType != type) {
          std::string newTypeStr = newType->getAsString(Context);
          std::string oldTypeStr = type->getAsString(Context);
          auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VD->getSourceRange());
          declaration.replace(declaration.find(oldTypeStr), oldTypeStr.length(), newTypeStr);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declaration);
        }
      } 
      } else if (auto *PD = Result.Nodes.getNodeAs<clang::ParmVarDecl>("ParmDecls")) {
      //Filter nodes in header files
      if (!PD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PD->getLocation()))
        return;
      //Get the source code text of target node
      auto type = PD->getType();
      if (!type)
        return;
      //Perform mutation on the source code text by applying string replacement
      if (existingTypes.size() > 0) {
        int choice = rand() % existingTypes.size();
        auto newType = existingTypes[choice];
        if (newType != type) {
          std::string newTypeStr = newType->getAsString(Context);
          std::string oldTypeStr = type->getAsString(Context);
          auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   PD->getSourceRange());
          declaration.replace(declaration.find(oldTypeStr), oldTypeStr.length(), newTypeStr);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(PD->getSourceRange()), declaration);
        }
      } 
      } else if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("TypeDecls")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Get the source code text of target node
      auto type = TD->getTypeForDecl();
      existingTypes.push_back(type);
    }
}.. 
    MutatorFrontendAction_234::MutatorASTConsumer_234::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varMatcher = varDecl().bind("VarDecls");
    DeclarationMatcher parmMatcher = parmVarDecl().bind("ParmDecls");
    DeclarationMatcher typeMatcher = typeDecl(hasName()).bind("TypeDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(parmMatcher, &callback);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.matchAST(Context);
}

void MutatorFrontendAction_234::Callback::run(const MatchFinder::MatchResult &Result) {
      //Check whether the matched AST node is the target node
      if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto type = VD->getType();
      if (!type)
        return;
      //Perform mutation on the source code text by applying string replacement
      if (existingTypes.size() > 0) {
        int choice = rand() % existingTypes.size();
        auto newType = existingTypes[choice];
        if (newType != type) {
          std::string newTypeStr = newType->getAsString(Context);
          std::string oldTypeStr = type->getAsString(Context);
          auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VD->getSourceRange());
          declaration.replace(declaration.find(oldTypeStr), oldTypeStr.length(), newTypeStr);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declaration);
        }
      } 
      } else if (auto *PD = Result.Nodes.getNodeAs<clang::ParmVarDecl>("ParmDecls")) {
      //Filter nodes in header files
      if (!PD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PD->getLocation()))
        return;
      //Get the source code text of target node
      auto type = PD->getType();
      if (!type)
        return;
      //Perform mutation on the source code text by applying string replacement
      if (existingTypes.size() > 0) {
        int choice = rand() % existingTypes.size();
        auto newType = existingTypes[choice];
        if (newType != type) {
          std::string newTypeStr = newType->getAsString(Context);
          std::string oldTypeStr = type->getAsString(Context);
          auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   PD->getSourceRange());
          declaration.replace(declaration.find(oldTypeStr), oldTypeStr.length(), newTypeStr);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(PD->getSourceRange()), declaration);
        }
      } 
      } else if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("TypeDecls")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Get the source code text of target node
      auto type = TD->getTypeForDecl();
      existingTypes.push_back(type);
    }
}