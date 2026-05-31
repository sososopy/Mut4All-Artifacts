//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_TypeAlias_With_Recursive_Struct_359
 */ 
class MutatorFrontendAction_359 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(359)

private:
    class MutatorASTConsumer_359 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_359(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TypeAliasTemplateDecl *> aliasDecls;
        std::vector<std::string> aliasNames;
        std::vector<std::string> templateParamNames;
    };
};

//source file
#include "../include/Mutator_Replace_TypeAlias_With_Recursive_Struct_359.h"

// ========================================================================================================
#define MUT359_OUTPUT 1

void MutatorFrontendAction_359::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto aliasDecl = MT->getTemplatedDecl();
      if (!aliasDecl) return;
      auto aliasName = aliasDecl->getNameAsString();
      auto aliasDef = aliasDecl->getUnderlyingType();
      //Check if the definition is a simple type (not a struct/class definition)
      if (aliasDef->isStructureType() || aliasDef->isClassType()) return;
      //Record the node information to be used in the mutation process
      aliasDecls.push_back(MT);
      aliasNames.push_back(aliasName);
      //Extract template parameter names
      auto templateParams = MT->getTemplateParameters();
      std::string paramName = "";
      if (templateParams && templateParams->size() > 0) {
        auto param = templateParams->getParam(0);
        if (param) paramName = param->getNameAsString();
      }
      templateParamNames.push_back(paramName);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("NonTemplateAliasDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto aliasName = MT->getNameAsString();
      auto aliasDef = MT->getUnderlyingType();
      //Check if the definition is a simple type (not a struct/class definition)
      if (aliasDef->isStructureType() || aliasDef->isClassType()) return;
      //Record the node information to be used in the mutation process
      aliasDecls.push_back(nullptr);
      aliasNames.push_back(aliasName);
      templateParamNames.push_back("");
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasDeclToMutate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto aliasDecl = MT->getTemplatedDecl();
      if (!aliasDecl) return;
      auto aliasName = aliasDecl->getNameAsString();
      //Find matching recorded alias
      for (size_t i = 0; i < aliasNames.size(); ++i) {
        if (aliasNames[i] == aliasName && aliasDecls[i] == MT) {
          //Perform mutation on the source code text by applying string replacement
          std::string newDef;
          if (templateParamNames[i].empty()) {
            newDef = "struct " + aliasName + " { int data; };";
          } else {
            newDef = "struct " + aliasName + " { " + templateParamNames[i] + " data; };";
          }
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(aliasDecl->getSourceRange()), newDef);
          break;
        }
      }
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("NonTemplateAliasDeclToMutate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto aliasName = MT->getNameAsString();
      //Find matching recorded alias
      for (size_t i = 0; i < aliasNames.size(); ++i) {
        if (aliasNames[i] == aliasName && aliasDecls[i] == nullptr) {
          //Perform mutation on the source code text by applying string replacement
          std::string newDef = "struct " + aliasName + " { int data; };";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), newDef);
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_359::MutatorASTConsumer_359::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = typeAliasTemplateDecl().bind("AliasDecl");
    DeclarationMatcher matcher2 = typeAliasDecl().bind("NonTemplateAliasDecl");
    DeclarationMatcher matcher3 = typeAliasTemplateDecl().bind("AliasDeclToMutate");
    DeclarationMatcher matcher4 = typeAliasDecl().bind("NonTemplateAliasDeclToMutate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.addMatcher(matcher3, &callback);
    matchFinder.addMatcher(matcher4, &callback);
    matchFinder.matchAST(Context);
}