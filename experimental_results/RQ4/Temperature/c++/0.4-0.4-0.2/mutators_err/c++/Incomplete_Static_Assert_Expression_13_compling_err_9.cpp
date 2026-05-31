//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include "llvm/Support/Casting.h"
#include <string>

using namespace clang;
using namespace clang::ast_matchers;

/**
 * incomplete_static_assert_expression_13
 */ 
class MutatorFrontendAction_13 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(13)

private:
    class MutatorASTConsumer_13 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_13(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/incomplete_static_assert_expression_13.h"

// ========================================================================================================
#define MUT13_OUTPUT 1

void MutatorFrontendAction_13::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (const auto *MT = Result.Nodes.getNodeAs<StaticAssertDecl>("StaticAssert")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      //Get the source code text of target node
      const Expr *messageExpr = MT->getMessage()->IgnoreParenCasts();
      if (const auto *SL = llvm::dyn_cast<StringLiteral>(messageExpr)) {
          std::string message = SL->getString().str();
          //Perform mutation on the source code text by applying string replacement
          if (!message.empty()) {
              std::string mutatedMessage = message.substr(0, message.size() - 1); // Remove last character
              std::string newStaticAssert = "static_assert(" + Lexer::getSourceText(CharSourceRange::getTokenRange(MT->getAssertExpr()->getSourceRange()), Result.Context->getSourceManager(), Result.Context->getLangOpts()).str() + ", \"" + mutatedMessage + "\");";
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), newStaticAssert);
          }
      }
    }
}
  
void MutatorFrontendAction_13::MutatorASTConsumer_13::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = staticAssertDecl().bind("StaticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}