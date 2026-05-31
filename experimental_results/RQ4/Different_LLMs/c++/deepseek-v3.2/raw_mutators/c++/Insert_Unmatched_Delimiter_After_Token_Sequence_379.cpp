//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unmatched_Delimiter_After_Token_Sequence_379
 */ 
class MutatorFrontendAction_379 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(379)

private:
    class MutatorASTConsumer_379 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_379(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Unmatched_Delimiter_After_Token_Sequence_379.h"

// ========================================================================================================
#define MUT379_OUTPUT 1

void MutatorFrontendAction_379::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      SourceLocation loc = FD->getLocation();
      SourceManager &SM = *Result.SourceManager;
      //Perform mutation on the source code text by applying string replacement
      //Find the '(' token after function name
      if (FD->hasBody()) {
        //Get the function declaration source range
        SourceRange declRange = FD->getSourceRange();
        //Get the source text
        std::string declText = stringutils::rangetoStr(SM, declRange);
        //Find the '(' position in the declaration
        size_t openParenPos = declText.find('(');
        if (openParenPos != std::string::npos) {
          //Insert unmatched ')' after '('
          declText.insert(openParenPos + 1, ")");
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(declRange), declText);
        }
      }
    }
}
  
void MutatorFrontendAction_379::MutatorASTConsumer_379::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}