//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Template_Argument_60
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Template_Template_Argument_60.h"

// ========================================================================================================
#define MUT60_OUTPUT 1

void MutatorFrontendAction_60::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateTemplateParmDecl>("TemplateTemplateParm")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace template template argument with a new template that has the same constraints but with a different template parameter list
      std::string mutatedDeclaration = declaration;
      size_t pos = mutatedDeclaration.find("template<");
      if (pos != std::string::npos) {
        size_t endPos = mutatedDeclaration.find(">", pos);
        if (endPos != std::string::npos) {
          std::string templateParams = mutatedDeclaration.substr(pos + 9, endPos - pos - 9);
          std::string newTemplateParams = "C int...";
          mutatedDeclaration.replace(pos + 9, templateParams.length(), newTemplateParams);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_60::MutatorASTConsumer_60::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTemplateParmDecl().bind("TemplateTemplateParm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}