//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_argument_mismatch_variadic_1
 */ 
class MutatorFrontendAction_1 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(1)

private:
    class MutatorASTConsumer_1 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_1(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/template_argument_mismatch_variadic_1.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isTemplateInstantiation()) {
        //Get the source code text of target node
        auto templateArgs = FD->getTemplateSpecializationArgs();
        if (templateArgs && templateArgs->size() > 1) {
          //Perform mutation on the source code text by applying string replacement
          std::string newTemplateArgs = FD->getNameInfo().getAsString() + "<";
          newTemplateArgs += templateArgs->get(0).getAsType().getAsString();
          newTemplateArgs += ">(";

          // Collect original arguments
          std::string originalArgs;
          for (unsigned i = 0; i < FD->getNumParams(); ++i) {
            if (i > 0) originalArgs += ", ";
            originalArgs += FD->getParamDecl(i)->getNameAsString();
          }
          newTemplateArgs += originalArgs + ");";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(FD->getSourceRange(), newTemplateArgs);
        }
      }
    }
}

void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasDescendant(templateArgumentList())).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}