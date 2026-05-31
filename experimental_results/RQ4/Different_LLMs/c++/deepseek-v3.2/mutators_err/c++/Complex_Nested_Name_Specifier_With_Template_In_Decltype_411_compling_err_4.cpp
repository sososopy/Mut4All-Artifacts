//header file
#pragma once
#include "Mutator_base.h"

/**
 * Complex_Nested_Name_Specifier_With_Template_In_Decltype_411
 */ 
class MutatorFrontendAction_411 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(411)

private:
    class MutatorASTConsumer_411 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_411(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::DecltypeType*> decltypeNodes;
        std::vector<const clang::DependentNameType*> dependentNameNodes;
        std::vector<const clang::TemplateDecl*> templateDecls;
    };
};

//source file
#include "../include/Complex_Nested_Name_Specifier_With_Template_In_Decltype_411.h"

// ========================================================================================================
#define MUT411_OUTPUT 1

void MutatorFrontendAction_411::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::DecltypeType>("decltypeExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocStart()))
        return;
      //Record the node information to be used in the mutation process
      decltypeNodes.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::DependentNameType>("dependentName")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocStart()))
        return;
      dependentNameNodes.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("templateDecl")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      templateDecls.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDecl")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto decltypeNodesInFunc = decltypeNodes;
      auto dependentNameNodesInFunc = dependentNameNodes;
      auto templateDeclsInFunc = templateDecls;
      //Clear vectors for next function
      decltypeNodes.clear();
      dependentNameNodes.clear();
      templateDecls.clear();

      if (decltypeNodesInFunc.empty() || dependentNameNodesInFunc.empty() || templateDeclsInFunc.empty())
        return;

      //Find a decltype that contains a dependent name specifier
      for (auto decltypeNode : decltypeNodesInFunc) {
        auto underlyingExpr = decltypeNode->getUnderlyingExpr();
        if (!underlyingExpr) continue;
        //Check if expression contains a dependent nested name specifier with template keyword
        std::string exprText = stringutils::rangetoStr(*(Result.SourceManager), underlyingExpr->getSourceRange());
        if (exprText.find("template") == std::string::npos) continue;

        //Find a dependent name type that could be used for mutation
        for (auto dependentNameNode : dependentNameNodesInFunc) {
          std::string dependentNameText = stringutils::rangetoStr(*(Result.SourceManager), dependentNameNode->getLocStart(), dependentNameNode->getLocEnd());
          //Check if this dependent name is already in the decltype expression
          if (exprText.find(dependentNameText) == std::string::npos) continue;

          //Find a template declaration that could be inserted
          for (auto templateDecl : templateDeclsInFunc) {
            std::string templateName = templateDecl->getNameAsString();
            //Check if this template is already present in the expression
            if (exprText.find(templateName) != std::string::npos) continue;

            //Check if template is nested within the dependent type
            auto templateContext = templateDecl->getDeclContext();
            if (!templateContext) continue;
            std::string contextName = templateDecl->getDeclName().getAsString();
            if (dependentNameText.find(contextName) == std::string::npos) continue;

            //Perform mutation on the source code text by applying string replacement
            //Insert template keyword and nested template name
            size_t templatePos = exprText.find("template");
            if (templatePos == std::string::npos) continue;
            size_t afterTemplate = exprText.find("::", templatePos);
            if (afterTemplate == std::string::npos) continue;
            std::string mutatedExpr = exprText;
            mutatedExpr.insert(afterTemplate + 2, "template " + templateName + "::");
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(underlyingExpr->getSourceRange()), mutatedExpr);
            return;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_411::MutatorASTConsumer_411::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher decltypeMatcher = decltypeType().bind("decltypeExpr");
    TypeMatcher dependentNameMatcher = dependentTemplateNameType().bind("dependentName");
    DeclarationMatcher templateMatcher = templateDecl(hasName("")).bind("templateDecl");
    DeclarationMatcher functionMatcher = functionDecl().bind("functionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(decltypeMatcher, &callback);
    matchFinder.addMatcher(dependentNameMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}