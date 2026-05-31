//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incorrect_Template_Parameter_List_302
 */ 
class MutatorFrontendAction_302 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(302)

private:
    class MutatorASTConsumer_302 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_302(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Incorrect_Template_Parameter_List_302.h"

// ========================================================================================================
#define MUT302_OUTPUT 1

void MutatorFrontendAction_302::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto templateText = stringutils::rangetoStr(*(Result.SourceManager),
                                                  TD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t pos = templateText.find("class...");
      if (pos != std::string::npos) {
        templateText.replace(pos, 8, "class ");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateText);
    }
}
  
void MutatorFrontendAction_302::MutatorASTConsumer_302::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl(has(templateParameterList(has(parameterPack())))).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}