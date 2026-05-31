//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Parameter_Pack_Expansion_Swap_90
 */ 
class MutatorFrontendAction_90 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(90)

private:
    class MutatorASTConsumer_90 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_90(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Template_Parameter_Pack_Expansion_Swap_90.h"
#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT90_OUTPUT 1

void MutatorFrontendAction_90::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateParameterList>("TemplateParameterList")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange(
              clang::SourceRange(MT->getBeginLoc(), MT->getEndLoc()), 
              *Result.SourceManager, 
              clang::LangOptions()
          ), 
          *Result.SourceManager
      );
      //Perform mutation on the source code text by applying string replacement
      // Swap the first two template parameters
      size_t firstComma = declaration.find(',');
      if (firstComma != std::string::npos) {
        size_t secondComma = declaration.find(',', firstComma + 1);
        if (secondComma != std::string::npos) {
          std::string firstParam = declaration.substr(0, firstComma);
          std::string secondParam = declaration.substr(firstComma + 1, secondComma - firstComma - 1);
          std::string rest = declaration.substr(secondComma + 1);
          declaration = secondParam + "," + firstParam + "," + rest;
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange(
          clang::SourceRange(MT->getBeginLoc(), MT->getEndLoc()), 
          *Result.SourceManager
      ), declaration);
    }
}
  
void MutatorFrontendAction_90::MutatorASTConsumer_90::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::templateParameterList().bind("TemplateParameterList");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}