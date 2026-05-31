```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Arguments_35
 */ 
class MutatorFrontendAction_35 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(35)

private:
    class MutatorASTConsumer_35 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_35(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Replace_Template_Arguments_35.h"

// ========================================================================================================
#define MUT35_OUTPUT 1

void MutatorFrontendAction_35::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record the template declaration
      templateDecls.push_back(TD);
    }
    else if (auto *TI = Result.Nodes.getNodeAs<clang::TemplateInstantiationDecl>("TemplateInstantiationDecl")) {
      //Filter nodes in header files
      if (!TI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TI->getLocation()))
        return;
      //Get the source code text of target node
      auto instantiation = stringutils::rangetoStr(*(Result.SourceManager), TI->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      for (const auto &TD : templateDecls) {
        if (TD->getNameAsString() == TI->getTemplateName().getAsTemplateDecl()->getNameAsString()) {
          // Replace template arguments
          std::string replacement = "typename int";
          instantiation.replace(instantiation.find("typename T"), 11, replacement);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(TI->getSourceRange()), instantiation);
        }
      }
    }
}
  
void MutatorFrontendAction_35::MutatorASTConsumer_35::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = templateDecl().bind("TemplateDecl");
    DeclarationMatcher matcher2 = templateInstantiationDecl().bind("TemplateInstantiationDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}