//header file
#pragma once
#include "Mutator_base.h"

/**
 * Attribute_Insertion_And_Corruption_368
 */ 
class MutatorFrontendAction_368 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(368)

private:
    class MutatorASTConsumer_368 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_368(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Attribute_Insertion_And_Corruption_368.h"

// ========================================================================================================
#define MUT368_OUTPUT 1

void MutatorFrontendAction_368::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TU")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto start_loc = MT->getBeginLoc();
      auto end_loc = MT->getEndLoc();
      auto source_range = clang::SourceRange(start_loc, end_loc);
      auto source_text = stringutils::rangetoStr(*(Result.SourceManager), source_range);
      //Perform mutation on the source code text by applying string replacement
      auto insert_loc = source_text.find_first_not_of(" \t\n\r");
      auto random_string = getrandom::getRandomString(10);
      auto mutated_text = "[][mut_368_" + random_string + "]" + source_text.substr(insert_loc);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(source_range, mutated_text);
    }
}
  
void MutatorFrontendAction_368::MutatorASTConsumer_368::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = translationUnitDecl().bind("TU");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}