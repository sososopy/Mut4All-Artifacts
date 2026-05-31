//header file
#pragma once
#include "Mutator_base.h"

/**
 * add_complex_constraint_to_template_664
 */ 
class MutatorFrontendAction_664 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(664)

private:
    class MutatorASTConsumer_664 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_664(Rewriter &R) : TheRewriter(R) {}
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
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Lexer.h"

using namespace clang;
using namespace clang::ast_matchers;

void MutatorFrontendAction_664::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionTemplates")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getDescribedFunctionTemplate()) {
        auto constraints = FD->getTrailingRequiresClause();
        if (constraints) {
          auto constraintText = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(constraints->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());
          std::string newConcept = "AdditionalConcept";
          std::string newConstraint = constraintText.str() + " && " + newConcept + "<T>";
          Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(constraints->getSourceRange()), newConstraint);
        }
      }
    }
}
  
void MutatorFrontendAction_664::MutatorASTConsumer_664::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasTrailingRequiresClause()).bind("FunctionTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}