//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_445
 */ 
class MutatorFrontendAction_445 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(445)

private:
    class MutatorASTConsumer_445 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_445(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Parameter_445.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT445_OUTPUT 1

void MutatorFrontendAction_445::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Get the template parameters
      auto templateParams = MT->getTemplateParameters();
      if (templateParams && templateParams->size() > 0) {
        //Get the types used in the same scope
        auto scope = MT->getParent();
        std::vector<std::string> typesInScope;
        for (auto decl : scope->decls()) {
          if (auto typeDecl = dyn_cast<TypeDecl>(decl)) {
            typesInScope.push_back(typeDecl->getNameAsString());
          }
        }
        if (typesInScope.size() > 0) {
          //Replace one of the template parameters with an existing type in the scope
          int indexToReplace = getrandom::getRandomIndex(templateParams->size() - 1);
          std::string replacementType = typesInScope[getrandom::getRandomIndex(typesInScope.size() - 1)];
          declaration.replace(declaration.find((*templateParams)[indexToReplace]->getNameAsString()), 
                             (*templateParams)[indexToReplace]->getNameAsString().size(), replacementType);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_445::MutatorASTConsumer_445::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}