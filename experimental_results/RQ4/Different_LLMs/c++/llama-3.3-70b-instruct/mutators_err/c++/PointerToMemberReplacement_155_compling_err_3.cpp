//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * PointerToMemberReplacement_155
 */ 
class MutatorFrontendAction_155 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(155)

private:
    class MutatorASTConsumer_155 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_155(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/PointerToMemberReplacement_155.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"

// ========================================================================================================
#define MUT155_OUTPUT 1

void MutatorFrontendAction_155::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::UnaryOperator>("UnaryOperator")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      if (MT->getOpcode() == UnaryOperatorKind::UO_AddrOf) {
        auto newDeclaration = declaration;
        newDeclaration.replace(newDeclaration.find("::"), 2, "->");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), newDeclaration);
      }
    }
}
  
void MutatorFrontendAction_155::MutatorASTConsumer_155::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = unaryOperator(unaryOperator(hasOperator(UnaryOperatorKind::UO_AddrOf))).bind("UnaryOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}