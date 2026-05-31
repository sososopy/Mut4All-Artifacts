//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unicode_Escape_Sequence_In_Identifier_309
 */ 
class MutatorFrontendAction_309 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(309)

private:
    class MutatorASTConsumer_309 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_309(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Insert_Unicode_Escape_Sequence_In_Identifier_309.h"

// ========================================================================================================
#define MUT309_OUTPUT 1

void MutatorFrontendAction_309::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::NamedDecl>("Identifier")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      std::string identifierName = MT->getNameAsString();
      if (identifierName.empty()) return;
      //(optional)Record the node information to be used in the mutation process
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedIdentifier;
      int dice = getrandom::getRandomIndex(3);
      switch (dice) {
        case 0:
          mutatedIdentifier = "\\u" + identifierName;
          break;
        case 1:
          mutatedIdentifier = "\\u{" + identifierName;
          break;
        case 2:
          mutatedIdentifier = identifierName.substr(0, identifierName.length()/2) + "\\u" + identifierName.substr(identifierName.length()/2);
          break;
        case 3:
          mutatedIdentifier = identifierName.substr(0, identifierName.length()/2) + "\\u{" + identifierName.substr(identifierName.length()/2);
          break;
        default:
          return;
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getLocation(), identifierName.length(), mutatedIdentifier);
    }
}
  
void MutatorFrontendAction_309::MutatorASTConsumer_309::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = namedDecl().bind("Identifier");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}