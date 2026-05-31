//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_Type_In_Template_Template_Parameter_391
 */ 
class MutatorFrontendAction_391 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(391)

private:
    class MutatorASTConsumer_391 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_391(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl *> templateDecls;
        std::vector<const clang::ClassTemplateDecl *> classTemplateDecls;
        std::vector<const clang::TypeDecl *> convertibleTypes;
    };
};

//source file
#include "../include/Replace_Template_Parameter_Type_In_Template_Template_Parameter_391.h"

// ========================================================================================================
#define MUT391_OUTPUT 1

void MutatorFrontendAction_391::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TT = Result.Nodes.getNodeAs<clang::TemplateTemplateParmDecl>("TemplateTemplateParam")) {
      //Filter nodes in header files
      if (!TT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TT->getLocation()))
        return;
      
      //Get the source code text of target node
      auto templateParamType = TT->getTemplateParameters()->getParam(0);
      if (!templateParamType || !isa<clang::NonTypeTemplateParmDecl>(templateParamType))
        return;
      
      auto nonTypeParam = dyn_cast<clang::NonTypeTemplateParmDecl>(templateParamType);
      if (!nonTypeParam)
        return;
      
      auto paramType = nonTypeParam->getType();
      if (!paramType->isClassType())
        return;
    }
}