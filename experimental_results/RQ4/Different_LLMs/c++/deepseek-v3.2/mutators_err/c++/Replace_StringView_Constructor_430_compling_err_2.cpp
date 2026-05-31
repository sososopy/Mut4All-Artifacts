//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_StringView_Constructor_430
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
#include "../include/Replace_StringView_Constructor_430.h"

// ========================================================================================================
#define MUT430_OUTPUT 1

void MutatorFrontendAction_430::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::StringLiteral>("StringLiteral")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      std::string originalLiteral = MT->getString().str();
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCode;
      mutatedCode += "namespace { static const volatile char arr[] = {";
      for (size_t i = 0; i < originalLiteral.size(); ++i) {
        mutatedCode += "'" + std::string(1, originalLiteral[i]) + "'";
        if (i != originalLiteral.size() - 1) mutatedCode += ",";
      }
      mutatedCode += ",'\\0'}; }\n";
      mutatedCode += "std::string_view sv = static_cast<const char*>(arr);";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), mutated1Code);
    }
}
  
void MutatorFrontendAction_430::MutatorASTConsumer_430::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = stringLiteral().bind("StringLiteral");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}