//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Default_Template_Argument_Containing_Lambda_Expression_468
 */ 
class MutatorFrontendAction_468 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(468)

private:
    class MutatorASTConsumer_468 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_468(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateTypeParmDecl*> candidateParams;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT468_OUTPUT 1

void MutatorFrontendAction_468::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateTypeParam")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      //(optional)Record the node information to be used in the mutation process
      candidateParams.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::NonTypeTemplateParmDecl>("NonTypeTemplateParam")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      //(optional)Record the node information to be used in the mutation process
      candidateParams.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateTemplateParmDecl>("TemplateTemplateParam")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      //(optional)Record the node information to be used in the mutation process
      candidateParams.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateDecl = MT;
      auto *templateParams = templateDecl->getTemplateParameters();
      if (!templateParams || templateParams->size() == 0)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string lambdaDefaultArg;
      for (unsigned i = 0; i < templateParams->size(); ++i) {
        auto *param = templateParams->getParam(i);
        if (param->hasDefaultArgument())
          continue;
        if (auto *typeParam = dyn_cast<TemplateTypeParmDecl>(param)) {
          // Check if constrained by a callable concept
          bool constrainedByCallable = false;
          std::string conceptName;
          if (typeParam->hasTypeConstraint()) {
            auto *constraint = typeParam->getTypeConstraint();
            if (constraint) {
              conceptName = constraint->getConstraintExpr()->getStmtClassName();
              if (conceptName.find("Predicate") != std::string::npos ||
                  conceptName.find("Invocable") != std::string::npos ||
                  conceptName.find("RegularInvocable") != std::string::npos) {
                constrainedByCallable = true;
              }
            }
          }
          if (constrainedByCallable) {
            lambdaDefaultArg = " = decltype([](int){ return true; })";
          } else {
            lambdaDefaultArg = " = decltype([](){ return 0; })";
          }
          Rewrite.InsertTextAfterToken(param->getEndLoc(), lambdaDefaultArg);
          break;
        }
        else if (auto *nonTypeParam = dyn_cast<NonTypeTemplateParmDecl>(param)) {
          auto paramType = nonTypeParam->getType();
          if (paramType->isFunctionPointerType() || paramType->isMemberFunctionPointerType()) {
            lambdaDefaultArg = " = [](){ return 0; }";
            Rewrite.InsertTextAfterToken(param->getEndLoc(), lambdaDefaultArg);
            break;
          }
        }
        else if (auto *templateTemplateParam = dyn_cast<TemplateTemplateParmDecl>(param)) {
          // According to specification, do not apply to template template parameters
          continue;
        }
      }
      //Replace the original AST node with the mutated one
    }
}
  
void MutatorFrontendAction_468::MutatorASTConsumer_468::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = templateTypeParmDecl().bind("TemplateTypeParam");
    DeclarationMatcher matcher2 = nonTypeTemplateParmDecl().bind("NonTypeTemplateParam");
    DeclarationMatcher matcher3 = templateTemplateParmDecl().bind("TemplateTemplateParam");
    DeclarationMatcher matcher4 = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.addMatcher(matcher3, &callback);
    matchFinder.addMatcher(matcher4, &callback);
    matchFinder.matchAST(Context);
}