//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_Types_473
 */ 
class MutatorFrontendAction_473 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(473)

private:
    class MutatorASTConsumer_473 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_473(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Replace_Template_Parameter_Types_473.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Type.h"

// ========================================================================================================
#define MUT473_OUTPUT 1

void MutatorFrontendAction_473::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT ||!Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = MT->getTemplateParameters();
      for (auto I = templateParams->begin(); I!= templateParams->end(); ++I) {
        if ((*I)->isTemplateParameter()) {
          // Replace type parameter with a different but valid type
          auto newType = clang::QualType(Result.Context, Result.Context->getInt32Type());
          Rewrite.ReplaceText((*I)->getSourceRange(), newType.getAsString());
        } else if (auto *NT = dyn_cast<clang::NonTypeTemplateParmDecl>(*I)) {
          // Replace non-type parameter with a different but valid value
          auto newValue = clang::IntegerLiteral::Create(Result.Context, 42, clang::tok::kw_int, Result.Context->getInt32Type());
          Rewrite.ReplaceText((*I)->getSourceRange(), newValue->getSourceRange().getBegin().printToString(Result.Context->getSourceManager()));
          // Use NT to avoid unused variable warning
          (void)NT;
        }
      }
    }
}
  
void MutatorFrontendAction_473::MutatorASTConsumer_473::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}