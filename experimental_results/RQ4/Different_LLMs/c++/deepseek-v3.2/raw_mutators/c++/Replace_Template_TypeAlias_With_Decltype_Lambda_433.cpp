//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_TypeAlias_With_Decltype_Lambda_433
 */ 
class MutatorFrontendAction_433 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(433)

private:
    class MutatorASTConsumer_433 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_433(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::LambdaExpr*> lambdas;
    };
};

//source file
#include "../include/Mutator_Replace_Template_TypeAlias_With_Decltype_Lambda_433.h"

// ========================================================================================================
#define MUT433_OUTPUT 1

void MutatorFrontendAction_433::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      lambdas.push_back(LE);
    }
    else if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("TypeAliasTemplateDecl")) {
      //Filter nodes in header files
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
        return;
      //Get the source code text of target node
      auto aliasDecl = TA->getTemplatedDecl();
      if (!aliasDecl) return;
      auto aliasName = aliasDecl->getNameAsString();
      auto aliasRange = TA->getSourceRange();
      auto aliasText = stringutils::rangetoStr(*(Result.SourceManager), aliasRange);
      
      //Check if alias is inside a template class/struct
      auto parent = TA->getDeclContext();
      if (!parent) return;
      auto parentRecord = dyn_cast<clang::CXXRecordDecl>(parent);
      if (!parentRecord || !parentRecord->getDescribedClassTemplate()) return;
      
      //Check if alias uses template parameter
      auto aliasType = aliasDecl->getUnderlyingType();
      bool usesTemplateParam = false;
      auto templateParams = parentRecord->getDescribedClassTemplate()->getTemplateParameters();
      for (auto param : *templateParams) {
        if (aliasType.getAsString().find(param->getNameAsString()) != std::string::npos) {
          usesTemplateParam = true;
          break;
        }
      }
      if (!usesTemplateParam) return;
      
      //Find lambda to use
      std::string lambdaName = "lambda";
      std::string lambdaDecl = "";
      if (!lambdas.empty()) {
        auto lambda = lambdas.back();
        auto lambdaRange = lambda->getSourceRange();
        lambdaDecl = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
        //Extract lambda variable name if possible
        if (lambda->getLambdaClass()->getDeclName().isIdentifier()) {
          lambdaName = lambda->getLambdaClass()->getNameAsString();
        }
      } else {
        //Introduce new generic lambda
        lambdaDecl = "auto lambda = [](auto){};";
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedAlias = "";
      auto templateParam = templateParams->getParam(0);
      if (templateParam) {
        mutatedAlias = "using " + aliasName + " = decltype(" + lambdaName + ".template operator()<" + templateParam->getNameAsString() + ">(0));";
      } else {
        mutatedAlias = "using " + aliasName + " = decltype(" + lambdaName + ".template operator()<T>(0));";
      }
      
      //Replace the original AST node with the mutated one
      if (!lambdaDecl.empty() && lambdas.empty()) {
        Rewrite.InsertTextBefore(aliasRange.getBegin(), lambdaDecl + "\n");
      }
      Rewrite.ReplaceText(aliasRange, mutatedAlias);
    }
}
  
void MutatorFrontendAction_433::MutatorASTConsumer_433::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher lambdaMatcher = lambdaExpr().bind("LambdaExpr");
    DeclarationMatcher aliasMatcher = typeAliasTemplateDecl().bind("TypeAliasTemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.addMatcher(aliasMatcher, &callback);
    matchFinder.matchAST(Context);
}