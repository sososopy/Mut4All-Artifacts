//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Invalid_Unicode_Escape_Sequence_309
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
#include "../include/add_invalid_unicode_escape_sequence_309.h"

// ========================================================================================================
#define MUT309_OUTPUT 1

void MutatorFrontendAction_309::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FP = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FP->getLocation()))
        return;
      if (FP->getNumParams() == 0)
        return;
      //Get the source code text of target node
      auto paramlist = stringutils::rangetoStr(*(Result.SourceManager),
                                               FP->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      paramlist += " \\Ud{4}";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FP->getSourceRange()), paramlist);
    }
}
  
void MutatorFrontendAction_309::MutatorASTConsumer_309::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}