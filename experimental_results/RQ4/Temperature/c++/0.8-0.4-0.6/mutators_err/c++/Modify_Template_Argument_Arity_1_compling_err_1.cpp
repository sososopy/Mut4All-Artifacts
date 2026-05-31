//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_argument_arity_1
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
#include "../include/modify_template_argument_arity_1.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MC = Result.Nodes.getNodeAs<clang::CallExpr>("TemplateCall")) {
      //Filter nodes in header files
      if (!MC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MC->getExprLoc()))
        return;

      if (auto *DRE = dyn_cast<DeclRefExpr>(MC->getCallee())) {
        if (auto *FD = dyn_cast<FunctionDecl>(DRE->getDecl())) {
          if (FD->isTemplateInstantiation()) {
            //Get the source code text of target node
            auto templateArgs = MC->getTemplateArgs();
            std::string newTemplateArgs;

            if (templateArgs.size() > 0) {
              // Add an extra template argument to create an arity mismatch
              newTemplateArgs = "<int, ";
              for (unsigned i = 0; i < templateArgs.size(); ++i) {
                if (i > 0) newTemplateArgs += ", ";
                newTemplateArgs += templateArgs[i].getAsType().getAsString();
              }
              newTemplateArgs += ">";
            } else {
              // Add a single template argument if none exist
              newTemplateArgs = "<int>";
            }

            //Perform mutation on the source code text by applying string replacement
            SourceRange templateRange = MC->getLAngleLoc().getLocWithOffset(-1), MC->getRAngleLoc().getLocWithOffset(1);
            Rewrite.ReplaceText(templateRange, newTemplateArgs);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(hasAncestor(functionDecl(isTemplateInstantiation()))).bind("TemplateCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}