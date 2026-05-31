//header file
#pragma once
#include "Mutator_base.h"

/**
 * incomplete_template_definition_29
 */ 
class MutatorFrontendAction_29 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(29)

private:
    class MutatorASTConsumer_29 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_29(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/incomplete_template_definition_29.h"

// ========================================================================================================
#define MUT29_OUTPUT 1

void MutatorFrontendAction_29::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto classDecl = CTD->getTemplatedDecl();
      if (!classDecl->isCompleteDefinition())
        return;
      
      auto classRange = classDecl->getSourceRange();
      auto classText = stringutils::rangetoStr(*(Result.SourceManager), classRange);
      
      //Perform mutation on the source code text by applying string replacement
      auto openBracePos = classText.find("{");
      if (openBracePos != std::string::npos) {
        classText.erase(openBracePos);
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(classRange), classText);
    }
}
  
void MutatorFrontendAction_29::MutatorASTConsumer_29::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}