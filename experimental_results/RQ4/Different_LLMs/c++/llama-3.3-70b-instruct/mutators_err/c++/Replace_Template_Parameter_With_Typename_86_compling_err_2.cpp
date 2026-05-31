//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_With_Typename_86
 */ 
class MutatorFrontendAction_86 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(86)

private:
    class MutatorASTConsumer_86 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_86(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Parameter_With_Typename_86.h"

// ========================================================================================================
#define MUT86_OUTPUT 1

void MutatorFrontendAction_86::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = MT->getTemplateParameters();
      for (auto it = templateParams.begin(); it != templateParams.end(); ++it) {
        if ((*it)->isTypeParameter()) {
          // Perform mutation on the source code text by applying string replacement
          std::string typenameName = "typename";
          std::string replacement = typenameName + " " + (*it)->getNameAsString();
          Rewrite.ReplaceText((*it)->getSourceRange(), replacement);
        }
      }
    }
}
  
void MutatorFrontendAction_86::MutatorASTConsumer_86::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}