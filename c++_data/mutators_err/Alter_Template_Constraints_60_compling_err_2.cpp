//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Template_Constraints_60
 */ 
class MutatorFrontendAction_60 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(60)

private:
    class MutatorASTConsumer_60 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_60(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/alter_template_constraints_60.h"

// ========================================================================================================
#define MUT60_OUTPUT 1

void MutatorFrontendAction_60::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto templateText = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = templateText;
      
      // Find the concept constraint and replace it
      size_t pos = mutatedText.find("template<C auto");
      if (pos != std::string::npos) {
          mutatedText.replace(pos, 9, "template<D auto");
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_60::MutatorASTConsumer_60::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl(has(templateParameterList())).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}