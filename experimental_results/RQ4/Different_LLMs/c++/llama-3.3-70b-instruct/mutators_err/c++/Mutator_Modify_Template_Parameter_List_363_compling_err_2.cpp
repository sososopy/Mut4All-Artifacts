//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * Modify_Template_Parameter_List_363
 */ 
class MutatorFrontendAction_363 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(363)

private:
    class MutatorASTConsumer_363 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_363(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Modify_Template_Parameter_List_363.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT363_OUTPUT 1

void MutatorFrontendAction_363::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string templateParams = declaration.substr(declaration.find('<') + 1, declaration.find('>') - declaration.find('<') - 1);
      std::vector<std::string> params;
      size_t start = 0;
      size_t end = templateParams.find(',');
      while (end != std::string::npos) {
        params.push_back(templateParams.substr(start, end - start));
        start = end + 1;
        end = templateParams.find(',', start);
      }
      params.push_back(templateParams.substr(start));
      // Randomly select an operation: add, remove, or change a template parameter
      int choice = getrandom::getRandomIndex(2);
      if (choice == 0) { // Add a new template parameter
        std::string newParam = "typename T" + std::to_string(params.size());
        params.push_back(newParam);
      } else if (choice == 1) { // Remove a template parameter
        if (!params.empty()) {
          params.erase(params.begin() + getrandom::getRandomIndex(params.size()));
        }
      } else { // Change the type of an existing template parameter
        if (!params.empty()) {
          int index = getrandom::getRandomIndex(params.size());
          params[index] = "typename U" + std::to_string(index);
        }
      }
      std::string newTemplateParams;
      for (const auto &param : params) {
        newTemplateParams += param + ", ";
      }
      newTemplateParams.pop_back();
      newTemplateParams.pop_back();
      declaration.replace(declaration.find('<') + 1, declaration.find('>') - declaration.find('<') - 1, newTemplateParams);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_363::MutatorASTConsumer_363::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}