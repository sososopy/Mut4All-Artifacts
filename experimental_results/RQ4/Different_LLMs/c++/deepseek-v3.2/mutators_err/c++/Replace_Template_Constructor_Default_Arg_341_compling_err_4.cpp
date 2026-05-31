//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Constructor_Default_Arg_341
 */ 
class MutatorFrontendAction_341 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(341)

private:
    class MutatorASTConsumer_341 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_341(Rewriter &R) : TheRewriter(R) {}
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
        std::string derivedClassName;
    };
};

//source file
#include "../include/Mutator_Replace_Template_Constructor_Default_Arg_341.h"

// ========================================================================================================
#define MUT341_OUTPUT 1

void MutatorFrontendAction_341::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("ConstructorDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto *recordDecl = MT->getParent();
      if (!recordDecl || !recordDecl->isTemplateDecl())
        return;
      auto *templateDecl = recordDecl->getDescribedClassTemplate();
      if (!templateDecl)
        return;
      //Check if class inherits from its template parameter
      bool inheritsFromTemplateParam = false;
      for (auto base : recordDecl->bases()) {
        auto baseType = base.getType();
        if (baseType.getAsString() == templateDecl->getTemplateParameters()->getParam(0)->getNameAsString()) {
          inheritsFromTemplateParam = true;
          break;
        }
      }
      if (!inheritsFromTemplateParam)
        return;
      //Check if constructor has default argument
      if (MT->getNumParams() == 0)
        return;
      auto param = MT->getParamDecl(0);
      if (!param->hasDefaultArg())
       return;
      //Record the node information to be used in the mutation process
      derivedClassName = recordDecl->getNameAsString();
      //Generate undeclared template name
      std::string undeclaredTemplateName = "C";
      if (derivedClassName.length() > 0 && derivedClassName[0] == 'B') {
        undeclaredTemplateName += derivedClassName.substr(1);
      } else {
        undeclaredTemplateName += derivedClassName;
      }
      //Get default argument source
      auto defaultArg = param->getDefaultArg();
      SourceRange defaultArgRange = defaultArg->getSourceRange();
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedDefaultArg = undeclaredTemplateName + "<" + derivedClassName + ">()";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(defaultArgRange, mutatedDefaultArg);
    }
}
  
void MutatorFrontendAction_341::MutatorASTConsumer_341::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(hasParent(cxxRecordDecl(isClassTemplate()))).bind("ConstructorDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}