```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_String_Literal_With_Std_String_View_430
 */ 
class MutatorFrontendAction_430 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(430)

private:
    class MutatorASTConsumer_430 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_430(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_String_Literal_With_Std_String_View_430.h"

// ========================================================================================================
#define MUT430_OUTPUT 1

void MutatorFrontendAction_430::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SL = Result.Nodes.getNodeAs<clang::StringLiteral>(("StringLiteral"))) {
      //Filter nodes in header files
      if (!SL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SL->getLocation()))
        return;
      //Get the source code text of target node
      std::string replacement = "std::string_view(" + SL->getString() + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(SL->getSourceRange(), replacement);
    }
}

void MutatorFrontendAction_430::MutatorASTConsumer_430::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = stringLiteral().bind("StringLiteral");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}