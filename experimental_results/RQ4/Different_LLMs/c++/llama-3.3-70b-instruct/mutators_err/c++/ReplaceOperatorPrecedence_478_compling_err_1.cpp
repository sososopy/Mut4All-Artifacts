//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceOperatorPrecedence_478
 */ 
class MutatorFrontendAction_478 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(478)

private:
    class MutatorASTConsumer_478 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_478(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ReplaceOperatorPrecedence_478.h"

// ========================================================================================================
#define REPLACEOPERATORPRECEDENCE_478_OUTPUT 1

void MutatorFrontendAction_478::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BE = Result.Nodes.getNodeAs<clang::BinaryOperator>(("BinaryOperator"))) {
      //Filter nodes in header files
      if (!BE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BE->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   BE->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Add parentheses around the left operand
      size_t pos = declaration.find_first_of("+-*/%<>=!&|^");
      if (pos != string::npos) {
        string left = declaration.substr(0, pos);
        string right = declaration.substr(pos);
        declaration = "(" + left + ")" + right;
      }
      // Add parentheses around the right operand
      pos = declaration.find_last_of("+-*/%<>=!&|^");
      if (pos != string::npos) {
        string left = declaration.substr(0, pos + 1);
        string right = declaration.substr(pos + 1);
        declaration = left + "(" + right + ")";
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BE->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_478::MutatorASTConsumer_478::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = binaryOperator().bind("BinaryOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}