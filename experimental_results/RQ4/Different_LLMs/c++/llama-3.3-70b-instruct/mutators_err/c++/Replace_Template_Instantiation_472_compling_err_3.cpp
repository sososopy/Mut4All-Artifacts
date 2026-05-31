//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Instantiation_472
 */ 
class MutatorFrontendAction_472 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(472)

private:
    class MutatorASTConsumer_472 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_472(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Instantiation_472.h"

// ========================================================================================================
#define MUT472_OUTPUT 1

void MutatorFrontendAction_472::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace template instantiation with a different template parameter
      std::string templateName = MT->getTemplateArgs().get(0).getAsType().getAsString();
      std::string templateParameter = MT->getTemplateArgs().get(0).getAsString();
      std::string newTemplateParameter = "float"; // Replace with a different type
      std::string mutatedDeclaration = declaration;
      mutatedDeclaration.replace(mutatedDeclaration.find(templateParameter), templateParameter.length(), newTemplateParameter);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_472::MutatorASTConsumer_472::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}