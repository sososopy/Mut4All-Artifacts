//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * Replace_Requires_Clause_With_Equivalent_Expression_56
 */ 
class MutatorFrontendAction_56 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(56)

private:
    class MutatorASTConsumer_56 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_56(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite, ASTContext &Context) : Rewrite(Rewrite), Context(Context) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        ASTContext &Context;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Requires_Clause_With_Equivalent_Expression_56.h"
#include "clang/AST/TemplateBase.h"
#include "clang/AST/DeclTemplate.h"

// ========================================================================================================
#define MUT56_OUTPUT 1

void MutatorFrontendAction_56::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>({"TemplateDecl"})) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Context.getLangOpts());
      // Check if the template has a requires clause
      if (const auto *CT = dyn_cast<const clang::ClassTemplateDecl>(MT)) {
        for (const auto &TParam : CT->getTemplateParametersAsWritten()) {
          if (const auto *Req = TParam.getConstraint()) {
            // Get the constraint expression
            auto constraintExpr = Req->getConstraintExpr();
            // Create a static assertion
            auto staticAssert = "static_assert(" + 
                clang::Lexer::getSourceText(
                  CharSourceRange::getTokenRange(constraintExpr->getSourceRange()), 
                  *Result.SourceManager, 
                  Context.getLangOpts()) + 
                ", \"Constraint not satisfied\");";
            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration + "\n" + staticAssert);
          }
        }
      } else if (const auto *FT = dyn_cast<const clang::FunctionTemplateDecl>(MT)) {
        if (const auto *Req = FT->getConstraint()) {
          // Get the constraint expression
          auto constraintExpr = Req->getConstraintExpr();
          // Create a static assertion
          auto staticAssert = "static_assert(" + 
              clang::Lexer::getSourceText(
                CharSourceRange::getTokenRange(constraintExpr->getSourceRange()), 
                *Result.SourceManager, 
                Context.getLangOpts()) + 
              ", \"Constraint not satisfied\");";
          // Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration + "\n" + staticAssert);
        }
      }
    }
}
  
void MutatorFrontendAction_56::MutatorASTConsumer_56::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = 
        clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    matcher = matcher.anyOf(
        clang::ast_matchers::classTemplateDecl().bind("TemplateDecl"),
        clang::ast_matchers::functionTemplateDecl().bind("TemplateDecl")
    );
    Callback callback(TheRewriter, Context);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}