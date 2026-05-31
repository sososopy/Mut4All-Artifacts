//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateParameterPackExpansion_Mutator_215
 */ 
class MutatorFrontendAction_215 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(215)

private:
    class MutatorASTConsumer_215 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_215(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/TemplateParameterPackExpansion_Mutator_215.h"

// ========================================================================================================
#define MUT215_OUTPUT 1

void MutatorFrontendAction_215::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateParameterList>("TemplateParameterPack")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace the original parameter pack expansion with a different number of arguments
      std::string mutatedDeclaration;
      if (MT->getNumTemplateParameters() == 1) {
        // Replace with two arguments
        mutatedDeclaration = declaration + ", " + declaration;
      } else if (MT->getNumTemplateParameters() == 2) {
        // Replace with one argument
        mutatedDeclaration = declaration.substr(0, declaration.find(","));
      } else {
        // Replace with a different number of arguments
        int numArgs = MT->getNumTemplateParameters() + 1;
        mutatedDeclaration = "";
        for (int i = 0; i < numArgs; i++) {
          mutatedDeclaration += declaration + ", ";
        }
        mutatedDeclaration.pop_back();
        mutatedDeclaration.pop_back();
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_215::MutatorASTConsumer_215::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateParameterList().bind("TemplateParameterPack");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}