//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Template_Argument_Substitution_254
 */ 
class MutatorFrontendAction_254 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(254)

private:
    class MutatorASTConsumer_254 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_254(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalid_Template_Argument_Substitution_254.h"

// ========================================================================================================
#define MUT254_OUTPUT 1

void MutatorFrontendAction_254::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>(("TemplateSpecialization"))) {
      //Filter nodes in header files
      if (!MT)
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getLocStart(), MT->getLocEnd());
      //Perform mutation on the source code text by applying string replacement
      // Replace the template argument with an undeclared identifier
      size_t templateArgStart = declaration.find('<');
      size_t templateArgEnd = declaration.find('>');
      if (templateArgStart != std::string::npos && templateArgEnd != std::string::npos) {
        std::string templateArg = declaration.substr(templateArgStart + 1, templateArgEnd - templateArgStart - 1);
        std::string newTemplateArg = "NewUnknown";
        declaration.replace(templateArgStart + 1, templateArg.length(), newTemplateArg);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getLocStart(), MT->getLocEnd()), declaration);
    }
}
  
void MutatorFrontendAction_254::MutatorASTConsumer_254::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}