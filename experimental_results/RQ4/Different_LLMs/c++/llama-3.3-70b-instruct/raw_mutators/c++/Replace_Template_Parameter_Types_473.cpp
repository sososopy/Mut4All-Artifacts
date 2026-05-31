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
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Template_Parameter_Types_473.h"

// ========================================================================================================
#define MUT473_OUTPUT 1

void MutatorFrontendAction_473::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = MT->getTemplateParameters();
      for (auto param : templateParams) {
        if (param->isTypeParameter()) {
          // Replace type parameter with a different but valid type
          auto newType = clang::QualType(Result.Context, Result.Context->getIntegerType());
          Rewrite.ReplaceText(param->getSourceRange(), newType.getAsString());
        } else if (param->isNonTypeParameter()) {
          // Replace non-type parameter with a different but valid value
          auto newValue = clang::IntegerLiteral::Create(Result.Context, 42, clang::tok::int_constant, Result.Context->getIntegerType());
          Rewrite.ReplaceText(param->getSourceRange(), newValue->getSourceRange().getBegin().printToString(Result.Context->getSourceManager()));
        }
      }
    }
}
  
void MutatorFrontendAction_473::MutatorASTConsumer_473::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}