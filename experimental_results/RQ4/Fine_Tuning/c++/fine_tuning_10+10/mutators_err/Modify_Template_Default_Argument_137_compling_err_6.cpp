//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Default_Argument_137
 */ 
class MutatorFrontendAction_137 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(137)

private:
    class MutatorASTConsumer_137 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_137(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Default_Argument_137.h"

// ========================================================================================================
#define MUT137_OUTPUT 1

void MutatorFrontendAction_137::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto templateParams = FTD->getTemplateParameters();
      for (auto *param : *templateParams) {
          if (auto *typeParam = llvm::dyn_cast<clang::TemplateTypeParmDecl>(param)) {
              if (typeParam->hasDefaultArgument()) {
                  //Perform mutation on the source code text by applying string replacement
                  std::string typeName = typeParam->getNameAsString();
                  std::string replacement = typeName + " = double"; // Example mutation
                  if (getrandom::getRandomIndex(2) == 0) {
                      replacement = typeName; // Remove default argument
                  }
                  //Replace the original AST node with the mutated one
                  Rewrite.ReplaceText(typeParam->getDefaultArgumentLoc(), replacement);
              }
          }
      }
    }
}
  
void MutatorFrontendAction_137::MutatorASTConsumer_137::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(has(templateTypeParmDecl(hasDefaultArgument())).bind("FuncTemplate"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}