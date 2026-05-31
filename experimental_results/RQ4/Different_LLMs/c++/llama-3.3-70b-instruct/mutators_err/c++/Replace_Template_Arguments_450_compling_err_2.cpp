//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Arguments_450
 */ 
class MutatorFrontendAction_450 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(450)

private:
    class MutatorASTConsumer_450 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_450(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Arguments_450.h"

// ========================================================================================================
#define MUT450_OUTPUT 1

void MutatorFrontendAction_450::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateArgument>(("TemplateArgument"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getAsType()->getLocStart()))
        return;
      //Get the source code text of target node
      auto arg = stringutils::rangetoStr(*(Result.SourceManager),
                                       MT->getAsType()->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Replace the original template argument with a new one
      int newArg = rand() % 100 + 1; // Generate a random integer between 1 and 100
      std::string newArgStr = std::to_string(newArg);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getAsType()->getSourceRange()), newArgStr);
    }
}
  
void MutatorFrontendAction_450::MutatorASTConsumer_450::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TemplateArgumentMatcher matcher = templateArgument().bind("TemplateArgument");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}