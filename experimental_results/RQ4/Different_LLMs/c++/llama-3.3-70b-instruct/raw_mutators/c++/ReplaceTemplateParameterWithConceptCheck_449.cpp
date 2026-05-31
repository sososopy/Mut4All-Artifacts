//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceTemplateParameterWithConceptCheck_449
 */ 
class MutatorFrontendAction_449 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(449)

private:
    class MutatorASTConsumer_449 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_449(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ReplaceTemplateParameterWithConceptCheck_449.h"

// ========================================================================================================
#define MUT449_OUTPUT 1

void MutatorFrontendAction_449::Callback::run(const MatchFinder::MatchResult &Result) {
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
      std::string templateParameterName = MT->getTemplateParameters()->getParam(0)->getNameAsString();
      std::string conceptCheckName = MT->getConceptRequirements()->getRequirement(0)->getNameAsString();
      std::string mutatedDeclaration = declaration;
      size_t pos = mutatedDeclaration.find(templateParameterName);
      while (pos != std::string::npos) {
        mutatedDeclaration.replace(pos, templateParameterName.length(), conceptCheckName);
        pos = mutatedDeclaration.find(templateParameterName, pos + conceptCheckName.length());
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_449::MutatorASTConsumer_449::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl(hasTemplateParameter(0)).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}