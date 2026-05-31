//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Argument_With_Statement_Expression_55
 */ 
class MutatorFrontendAction_55 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(55)
    
    private:
      class MutatorASTConsumer_55 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_55(Rewriter &R) : TheRewriter(R) {}
          void HandleTranslationUnit(ASTContext &Context) override;
        private:
          Rewriter &TheRewriter;
      };
      
      class Callback : public MatchFinder::MatchCallback {
        public:
          Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
          void run(const MatchFinder::MatchResult &Result) override;
        private:
          Rewriter &Rewrite;
          //Necessary node information record used in the mutation process
      };
    };

//source file
#include "../include/replace_default_argument_with_statement_expression_55.h"

// ========================================================================================================
#define MUT55_OUTPUT 1

void MutatorFrontendAction_55::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ParmVarDecl>("DefaultArgs")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->hasDefaultArg())
        return;
      auto default_expr = DL->getDefaultArg();
      if (!default_expr)
        return;
      auto default_arg = stringutils::rangetoStr(*(Result.SourceManager),
                                                 default_expr->getSourceRange());
      llvm::outs() << default_arg << '\n';
      auto new_default_arg = "/*mut55*/({ " + default_arg + "; })";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                              default_expr->getSourceRange()),
                          new_default_arg);
    }
  }

void MutatorFrontendAction_55::MutatorASTConsumer_55::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = parmVarDecl(hasDefaultArgument()).bind("DefaultArgs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}