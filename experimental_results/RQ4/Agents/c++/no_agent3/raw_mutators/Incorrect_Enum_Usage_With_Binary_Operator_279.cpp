//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incorrect_Enum_Usage_With_Binary_Operator_279
 */ 
class MutatorFrontendAction_279 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(279)

private:
    class MutatorASTConsumer_279 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_279(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Incorrect_Enum_Usage_With_Binary_Operator_279.h"

// ========================================================================================================
#define MUT279_OUTPUT 1

void MutatorFrontendAction_279::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;

      if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                       BO->getExprLoc()))
          return;
        
        //Get the source code text of target node
        auto binaryOpText = stringutils::rangetoStr(*(Result.SourceManager),
                                                    BO->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        auto mutatedText = binaryOpText + " += \"\""; // Intentionally incorrect
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_279::MutatorASTConsumer_279::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher enumMatcher = enumDecl().bind("EnumDecl");
    StatementMatcher binaryOpMatcher = binaryOperator(hasOperatorName("+=")).bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.addMatcher(binaryOpMatcher, &callback);
    matchFinder.matchAST(Context);
}