//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/ExprCXX.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Expr.h" // Added to resolve incomplete type issue

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Malform_Requires_Clause_116
 */ 
class MutatorFrontendAction_116 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(116)

private:
    class MutatorASTConsumer_116 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_116(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/malform_requires_clause_116.h"

// ========================================================================================================
#define MUT116_OUTPUT 1

void MutatorFrontendAction_116::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (const auto *RC = Result.Nodes.getNodeAs<RequiresExpr>("RequiresClause")) {
      //Filter nodes in header files
      if (!Result.Context->getSourceManager().isWrittenInMainFile(
                     RC->getRequiresKWLoc()))
        return;

      //Get the source code text of target node
      auto requiresText = stringutils::rangetoStr(*(Result.SourceManager),
                                                  RC->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t pos = requiresText.find("requires");
      if (pos != std::string::npos) {
          requiresText.replace(pos, std::string("requires").length(), "requires (int;");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RC->getSourceRange()), requiresText);
    }
}
  
void MutatorFrontendAction_116::MutatorASTConsumer_116::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = expr(hasType(asString("RequiresExpr"))).bind("RequiresClause");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}