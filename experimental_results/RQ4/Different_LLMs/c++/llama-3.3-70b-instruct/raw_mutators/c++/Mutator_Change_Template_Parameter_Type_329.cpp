//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Template_Parameter_Type_329
 */ 
class MutatorFrontendAction_Change_Template_Parameter_Type_329 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Change_Template_Parameter_Type_329)

private:
    class MutatorASTConsumer_Change_Template_Parameter_Type_329 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Change_Template_Parameter_Type_329(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_Change_Template_Parameter_Type_329.h"

// ========================================================================================================
#define MUT_CHANGE_TEMPLATE_PARAMETER_TYPE_329_OUTPUT 1

void MutatorFrontendAction_Change_Template_Parameter_Type_329::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = MT->getTemplateParameters();
      for (auto param : templateParams) {
        if (auto typeParam = dyn_cast<TemplateTypeParmDecl>(param)) {
          //Perform mutation on the source code text by applying string replacement
          std::string typeName = typeParam->getNameAsString();
          std::string newTypeName = "const " + typeName;
          Rewrite.ReplaceText(typeParam->getNameLoc(), typeName, newTypeName);
        }
      }
    }
}
  
void MutatorFrontendAction_Change_Template_Parameter_Type_329::MutatorASTConsumer_Change_Template_Parameter_Type_329::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}