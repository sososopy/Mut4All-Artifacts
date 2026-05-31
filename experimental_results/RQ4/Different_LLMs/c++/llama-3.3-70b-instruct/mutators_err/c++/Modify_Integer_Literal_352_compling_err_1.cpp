//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Integer_Literal_352
 */ 
class MutatorFrontendAction_352 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(352)

private:
    class MutatorASTConsumer_352 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_352(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Integer_Literal_352.h"

// ========================================================================================================
#define MUT352_OUTPUT 1

void MutatorFrontendAction_352::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *IL = Result.Nodes.getNodeAs<clang::IntegerLiteral>(("IntegerLiteral"))) {
      //Filter nodes in header files
      if (!IL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IL->getLocation()))
        return;
      //Get the source code text of target node
      std::string integerLiteral = stringutils::rangetoStr(*(Result.SourceManager), IL->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      int value = std::stoi(integerLiteral);
      int mutatedValue = -value; // Change the sign of the integer literal
      std::string mutatedIntegerLiteral = std::to_string(mutatedValue);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(IL->getSourceRange()), mutatedIntegerLiteral);
    }
}

void MutatorFrontendAction_352::MutatorASTConsumer_352::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = integerLiteral().bind("IntegerLiteral");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}