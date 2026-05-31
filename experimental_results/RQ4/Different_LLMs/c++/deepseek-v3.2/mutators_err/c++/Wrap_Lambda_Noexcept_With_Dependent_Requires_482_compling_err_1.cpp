//header file
#pragma once
#include "Mutator_base.h"

/**
 * Wrap_Lambda_Noexcept_With_Dependent_Requires_482
 */ 
class MutatorFrontendAction_482 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(482)

private:
    class MutatorASTConsumer_482 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_482(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateTypeParmType*> templateParams;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT482_OUTPUT 1

void MutatorFrontendAction_482::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      //Check if lambda is inside a template context
      const clang::DeclContext* parent = MT->getDecl()->getDeclContext();
      bool inTemplate = false;
      std::string paramName = "";
      
      while (parent) {
        if (const clang::FunctionDecl* FD = clang::dyn_cast<clang::FunctionDecl>(parent)) {
          if (FD->isTemplateDecl()) {
            inTemplate = true;
            //Find a template parameter type
            const clang::TemplateDecl* TD = FD->getTemplateDecl();
            if (TD) {
              const clang::TemplateParameterList* params = TD->getTemplateParameters();
              if (params && params->size() > 0) {
                //Use first template type parameter
                const clang::Decl* firstParam = params->getParam(0);
                if (const clang::TemplateTypeParmDecl* TTPD = clang::dyn_cast<clang::TemplateTypeParmDecl>(firstParam)) {
                  paramName = TTPD->getNameAsString();
                  if (paramName.empty()) paramName = "T";
                }
              }
            }
            break;
          }
        }
        else if (const clang::CXXRecordDecl* RD = clang::dyn_cast<clang::CXXRecordDecl>(parent)) {
          if (RD->getDescribedTemplate()) {
            inTemplate = true;
            const clang::TemplateDecl* TD = RD->getDescribedTemplate();
            if (TD) {
              const clang::TemplateParameterList* params = TD->getTemplateParameters();
              if (params && params->size() > 0) {
                const clang::Decl* firstParam = params->getParam(0);
                if (const clang::TemplateTypeParmDecl* TTPD = clang::dyn_cast<clang::TemplateTypeParmDecl>(firstParam)) {
                  paramName = TTPD->getNameAsString();
                  if (paramName.empty()) paramName = "T";
                }
              }
            }
            break;
          }
        }
        parent = parent->getParent();
      }
      
      //Also check if lambda itself is generic (has auto parameters)
      const clang::CXXRecordDecl* lambdaClass = MT->getLambdaClass();
      if (lambdaClass && lambdaClass->isGenericLambda()) {
        inTemplate = true;
        paramName = "auto";
      }
      
      if (!inTemplate) return;
      
      //Get the source code text of target node
      auto lambdaRange = MT->getSourceRange();
      std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      
      //Find location to insert noexcept specification
      size_t paramEndPos = lambdaText.find(')');
      if (paramEndPos == std::string::npos) return;
      
      //Check if lambda already has noexcept
      bool hasNoexcept = false;
      size_t noexceptPos = lambdaText.find("noexcept", paramEndPos);
      if (noexceptPos != std::string::npos && noexceptPos < lambdaText.find('{')) {
        hasNoexcept = true;
      }
      
      //Construct new noexcept specification
      std::string noexceptSpec = "noexcept(requires { requires requires { typename " + paramName + "::x; }; })";
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText;
      if (hasNoexcept) {
        //Replace existing noexcept
        size_t noexceptStart = lambdaText.find("noexcept", paramEndPos);
        size_t noexceptEnd = lambdaText.find(')', noexceptStart) + 1;
        mutatedText = lambdaText.substr(0, noexceptStart) + noexceptSpec + lambdaText.substr(noexceptEnd);
      } else {
        //Insert after parameters
        mutatedText = lambdaText.substr(0, paramEndPos + 1) + " " + noexceptSpec + lambdaText.substr(paramEndPos + 1);
      }
      
      mutatedText = "/*mut482*/" + mutatedText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), mutatedText);
    }
}
  
void MutatorFrontendAction_482::MutatorASTConsumer_482::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}