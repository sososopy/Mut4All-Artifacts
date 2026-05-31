//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Instantiation_With_Default_Arguments_62
 */ 
class MutatorFrontendAction_62 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(62)

private:
    class MutatorASTConsumer_62 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_62(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl *> templates;
    };
};

//source file
#include "../include/template_instantiation_with_default_arguments_62.h"

// ========================================================================================================
#define MUT62_OUTPUT 1

void MutatorFrontendAction_62::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplates")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FTD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      if (FTD->getTemplateParameters()->size() >= 2) {
        std::string newDeclaration = declaration;
        newDeclaration.insert(newDeclaration.find(">"), ", typename U = int, typename V = double");
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), newDeclaration);
        templates.push_back(FTD);
      }
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecls")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (!FD->isTemplateInstantiation())
        return;

      for (const auto *FTD : templates) {
        if (FD->getPrimaryTemplate() == FTD) {
          //Get the source code text of target node
          auto instantiation = stringutils::rangetoStr(*(Result.SourceManager),
                                                       FD->getSourceRange());

          //Perform mutation on the source code text by applying string replacement
          std::string newInstantiation = instantiation;
          newInstantiation.insert(newInstantiation.find("("), "<char>");

          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), newInstantiation);
        }
      }
    }
}
  
void MutatorFrontendAction_62::MutatorASTConsumer_62::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = functionTemplateDecl().bind("FunctionTemplates");
    DeclarationMatcher instantiationMatcher = functionDecl(isTemplateInstantiation()).bind("FunctionDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(instantiationMatcher, &callback);
    matchFinder.matchAST(Context);
}