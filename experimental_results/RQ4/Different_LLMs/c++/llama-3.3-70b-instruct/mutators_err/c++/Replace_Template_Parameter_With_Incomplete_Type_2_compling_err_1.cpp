//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_With_Incomplete_Type_2
 */ 
class MutatorFrontendAction_Replace_Template_Parameter_With_Incomplete_Type_2 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Replace_Template_Parameter_With_Incomplete_Type_2)

private:
    class MutatorASTConsumer_Replace_Template_Parameter_With_Incomplete_Type_2 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Replace_Template_Parameter_With_Incomplete_Type_2(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Parameter_With_Incomplete_Type_2.h"

// ========================================================================================================
#define REPLACE_TEMPLATE_PARAMETER_WITH_INCOMPLETE_TYPE_2_OUTPUT 1

void MutatorFrontendAction_Replace_Template_Parameter_With_Incomplete_Type_2::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = MT->getTemplateParameters();
      if (templateParams) {
        for (auto param : *templateParams) {
          if (param->isTypeParameter()) {
            //Perform mutation on the source code text by applying string replacement
            std::string replacement = "IncompleteClass";
            std::string original = param->getNameAsString();
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(param->getNameSourceLocation(), param->getNameSourceLocation() + original.size()), replacement);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_Replace_Template_Parameter_With_Incomplete_Type_2::MutatorASTConsumer_Replace_Template_Parameter_With_Incomplete_Type_2::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}