//header file
#pragma once
#include "Mutator_base.h"

/**
 * Rename_Type_Operator_40
 */ 
class MutatorFrontendAction_40 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(40)

private:
    class MutatorASTConsumer_40 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_40(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> existingTypes;
    };
};

//source file
#include "../include/Rename_Type_Operator_40.h"

// ========================================================================================================
#define MUT40_OUTPUT 1

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeLoc>("Type")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getType().getLocStart().getLocWithOffset(0)))
        return;
      //Get the source code text of target node
      std::string typeName = MT->getType().getAsString();
      //Record existing type names
      if (std::find(existingTypes.begin(), existingTypes.end(), typeName) == existingTypes.end()) {
        existingTypes.push_back(typeName);
      }
      //Perform mutation on the source code text by applying string replacement
      std::string replacementType;
      do {
        replacementType = existingTypes[rand() % existingTypes.size()];
      } while (replacementType == typeName);
      std::string mutatedCode = replacementType;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getType().getLocStart().getLocWithOffset(0), MT->getType().getLocEnd().getLocWithOffset(0), mutatedCode);
    }
}

void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = type().bind("Type");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}