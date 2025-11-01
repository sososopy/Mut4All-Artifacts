//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Argument_Constraints_295
 */ 
class MutatorFrontendAction_295 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(295)

private:
    class MutatorASTConsumer_295 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_295(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_argument_constraints_295.h"

// ========================================================================================================
#define MUT295_OUTPUT 1

void MutatorFrontendAction_295::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NTTP = Result.Nodes.getNodeAs<clang::NonTypeTemplateParmDecl>("NonTypeTemplateParm")) {
      //Filter nodes in header files
      if (!NTTP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NTTP->getLocation()))
        return;

      //Get the source code text of target node
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), NTTP->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = sourceText;
      if (sourceText.find("C") != std::string::npos) {
          mutatedText.replace(sourceText.find("C"), 1, "(C && sizeof...(auto) > 0)");
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(NTTP->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_295::MutatorASTConsumer_295::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = nonTypeTemplateParmDecl(hasType(conceptSpecializationType())).bind("NonTypeTemplateParm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}