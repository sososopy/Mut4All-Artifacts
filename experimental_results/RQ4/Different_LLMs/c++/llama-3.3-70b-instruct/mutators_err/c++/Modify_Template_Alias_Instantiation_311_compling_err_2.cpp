//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Alias_Instantiation_311
 */ 
class MutatorFrontendAction_311 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(311)

private:
    class MutatorASTConsumer_311 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_311(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Alias_Instantiation_311.h"

// ========================================================================================================
#define MUT311_OUTPUT 1

void MutatorFrontendAction_311::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateAliasDecl>("TemplateAlias")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                               MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Remove typename keyword if present
      if (declaration.find("typename") != std::string::npos) {
        declaration.replace(declaration.find("typename"), 8, "");
      } 
      // Add typename keyword if not present
      else {
        declaration.insert(declaration.find("::"), "typename ");
      }
      // Modify type parameters
      int choice = getrandom::getRandomIndex(1);
      if (choice) {
        declaration.replace(declaration.find("int"), 3, "double");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_311::MutatorASTConsumer_311::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateAliasDecl().bind("TemplateAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}