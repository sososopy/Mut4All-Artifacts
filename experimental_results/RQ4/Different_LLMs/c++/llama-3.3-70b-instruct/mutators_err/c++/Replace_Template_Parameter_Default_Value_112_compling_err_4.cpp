//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_Default_Value_112
 */ 
class MutatorFrontendAction_112 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(112)

private:
    class MutatorASTConsumer_112 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_112(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Parameter_Default_Value_112.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT112_OUTPUT 1

void MutatorFrontendAction_112::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Find the default value of the template parameter
      auto templateParams = MT->getTemplateParameters();
      for (auto param = templateParams->begin(); param != templateParams->end(); ++param) {
        if (auto *tparam = dyn_cast<TemplateTypeParmType>((*param)->getType())) {
          if (tparam->hasDefaultArgument()) {
            auto defaultValue = tparam->getDefaultArgument();
            // Replace the default value with a different valid expression
            // For example, replace the default value with a lambda expression
            auto newDefaultValue = "decltype([](char ch) {return ch != 'a';})";
            declaration.replace(declaration.find(defaultValue->getAsString()), defaultValue->getAsString().size(), newDefaultValue);
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_112::MutatorASTConsumer_112::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}