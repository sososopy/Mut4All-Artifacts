//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Argument_Usage_29
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
        std::set<std::string> modifiedTemplates; // Track modified templates to avoid duplicate mutations
    };
};

//source file
#include "../include/Modify_Template_Argument_Usage_29.h"

// ========================================================================================================
#define MUT29_OUTPUT 1

void MutatorFrontendAction_29::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateType")) {
      //Filter nodes in header files
      if (!VT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Result.Context->getSourceManager().getExpansionLoc(VT->getTemplateNameLoc())))
        return;

      // Get the source code text of target node
      auto templateName = VT->getTemplateName().getAsTemplateDecl()->getNameAsString();
      if (modifiedTemplates.find(templateName) != modifiedTemplates.end())
        return; // Skip if already modified

      // Perform mutation on the source code text by applying string replacement
      std::string newType = "struct NewType {};"; // Add new type at the beginning of the file
      std::string replacement = templateName + "<";
      for (unsigned i = 0; i < VT->getNumArgs(); ++i) {
          replacement += VT->getArg(i).getAsType().getAsString() + ", ";
      }
      replacement += "NewType>";

      // Replace the original AST node with the mutated one
      Rewrite.InsertTextBefore(Result.Context->getSourceManager().getExpansionLoc(VT->getTemplateNameLoc()), newType + "\n");
      Rewrite.ReplaceText(VT->getSourceRange(), replacement);
      modifiedTemplates.insert(templateName);
    }
}
  
void MutatorFrontendAction_29::MutatorASTConsumer_29::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}