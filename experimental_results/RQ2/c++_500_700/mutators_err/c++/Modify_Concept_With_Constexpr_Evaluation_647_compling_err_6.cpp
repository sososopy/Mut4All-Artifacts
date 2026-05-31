//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/AST/StmtCXX.h"
#include "clang/AST/ExprCXX.h"

/**
 * modify_concept_with_constexpr_evaluation_647
 */ 
class MutatorFrontendAction_647 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(647)

private:
    class MutatorASTConsumer_647 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_647(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_concept_with_constexpr_evaluation_647.h"

// ========================================================================================================
#define MUT647_OUTPUT 1

void MutatorFrontendAction_647::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *CTD = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;

      if (const auto *requiresExpr = Result.Nodes.getNodeAs<clang::RequiresExpr>("RequiresExpr")) {
        auto sourceRange = requiresExpr->getSourceRange();
        std::string originalCode = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(sourceRange), *Result.SourceManager, clang::LangOptions()).str();

        std::string constexprCode = "constexpr bool is_valid = sizeof(T) > 0; is_valid;";
        std::string mutatedCode = originalCode + " " + constexprCode;

        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(sourceRange), mutatedCode);
      }
    }
}
  
void MutatorFrontendAction_647::MutatorASTConsumer_647::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("ConceptDecl");
    StatementMatcher requiresMatcher = requiresExpr().bind("RequiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(requiresMatcher, &callback);
    matchFinder.matchAST(Context);
}