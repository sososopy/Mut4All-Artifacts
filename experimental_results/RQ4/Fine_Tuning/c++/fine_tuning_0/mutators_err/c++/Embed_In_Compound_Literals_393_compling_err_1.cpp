//header file
#pragma once
#include "Mutator_base.h"

/**
 * Embed_In_Compound_Literals_393
 */ 
class MutatorFrontendAction_393 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(393)

private:
    class MutatorASTConsumer_393 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_393(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Embed_In_Compound_Literals_393.h"

// ========================================================================================================
#define MUT393_OUTPUT 1

void MutatorFrontendAction_393::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CompoundLiteralExpr>("CompoundLiteral")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto compoundLiteral = stringutils::rangetoStr(
          *(Result.SourceManager), MT->getSourceRange());
      llvm::outs() << compoundLiteral << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (compoundLiteral.find('{') != string::npos) {
        compoundLiteral.insert(compoundLiteral.find('{') + 1,
                               "#embed \"mut393\"");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), compoundLiteral);
    }
}
  
void MutatorFrontendAction_393::MutatorASTConsumer_393::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = compoundLiteralExpr().bind("CompoundLiteral");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}