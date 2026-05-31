//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Tooling/Tooling.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/StmtCXX.h"
#include "clang/AST/ExprCXX.h"

/**
 * modify_concept_requirement_689
 */ 
class MutatorFrontendAction_689 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(689)

private:
    class MutatorASTConsumer_689 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_689(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_concept_requirement_689.h"

// ========================================================================================================
#define MUT689_OUTPUT 1

void MutatorFrontendAction_689::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (const auto *RC = Result.Nodes.getNodeAs<clang::RequiresExpr>("RequiresClause")) {
      //Filter nodes in header files
      if (!RC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RC->getRequiresKWLoc()))
        return;
      //Get the source code text of target node
      auto originalText = stringutils::rangetoStr(*(Result.SourceManager), RC->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string modifiedText = originalText;
      size_t pos = modifiedText.find("convertible_to<bool>");
      if (pos != std::string::npos) {
          modifiedText.insert(pos + std::string("convertible_to<bool>").length(), "; { __lhs < __rhs } -> convertible_to<int>");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RC->getSourceRange()), modifiedText);
    }
}
  
void MutatorFrontendAction_689::MutatorASTConsumer_689::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = requiresExpr().bind("RequiresClause");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}