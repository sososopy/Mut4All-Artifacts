//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/ExprCXX.h"
#include "clang/AST/ExprConcepts.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Concept_Specialization_Argument_Mutation_91
 */ 
class MutatorFrontendAction_91 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(91)

private:
    class MutatorASTConsumer_91 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_91(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/concept_specialization_argument_mutation_91.h"

// ========================================================================================================
#define MUT91_OUTPUT 1

void MutatorFrontendAction_91::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *CTSD = Result.Nodes.getNodeAs<ConceptSpecializationExpr>("ConceptSpecialization")) {
      if (!Result.Context->getSourceManager().isWrittenInMainFile(CTSD->getBeginLoc()))
        return;

      const auto &args = CTSD->getTemplateArgsAsWritten()->arguments();
      if (args.size() == 0) return;

      std::string mutatedArgs;
      for (size_t i = 0; i < args.size(); ++i) {
          if (i == 0) {
              mutatedArgs += "int"; // Replace first argument with a non-matching type
          } else {
              mutatedArgs += args[i].getArgument().getAsType().getAsString();
          }
          if (i < args.size() - 1) {
              mutatedArgs += ", ";
          }
      }
      auto original = Lexer::getSourceText(CharSourceRange::getTokenRange(CTSD->getSourceRange()), *(Result.SourceManager), LangOptions());
      auto mutated = original.str();
      auto pos = mutated.find('<');
      if (pos != std::string::npos) {
          mutated.replace(pos + 1, original.find('>') - pos - 1, mutatedArgs);
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CTSD->getSourceRange()), mutated);
    }
}
  
void MutatorFrontendAction_91::MutatorASTConsumer_91::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = expr(anyOf(conceptSpecializationExpr())).bind("ConceptSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}