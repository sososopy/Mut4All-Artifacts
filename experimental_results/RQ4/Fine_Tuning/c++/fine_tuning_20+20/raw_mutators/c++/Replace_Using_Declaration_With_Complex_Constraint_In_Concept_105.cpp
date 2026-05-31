//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Using_Declaration_With_Complex_Constraint_In_Concept_105
 */ 
class MutatorFrontendAction_105 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(105)
    
    private:
      class MutatorASTConsumer_105 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_105(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_using_declaration_with_complex_constraint_in_concept_105.h"

// ========================================================================================================
#define MUT105_OUTPUT 1

void MutatorFrontendAction_105::Callback::run(const MatchFinder::MatchResult &Result) {
  if (const auto *RE = Result.Nodes.getNodeAs<clang::RequiresExpr>("Requires")) {
    if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(
                   RE->getBeginLoc()))
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), RE->getSourceRange());
    llvm::outs() << content;
    auto first_brace = content.find('{');
    auto last_brace = content.rfind('}');
    if (first_brace == string::npos || last_brace == string::npos)
      return;
    auto body = content.substr(first_brace + 1, last_brace - first_brace - 1);
    llvm::outs() << body;
    if (body.find("using") == string::npos)
      return;
    if (body.find("begin") == string::npos || body.find("end") == string::npos)
      return;
    auto new_body =
        "requires std::is_convertible_v<decltype(std::begin(t)), std::input_iterator<T>> && std::is_convertible_v<decltype(std::end(t)), std::input_iterator<T>>";
    content.replace(first_brace + 1, last_brace - first_brace - 1, new_body);
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(RE->getSourceRange()),
                        content);
  }
}

void MutatorFrontendAction_105::MutatorASTConsumer_105::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = requiresExpr().bind("Requires");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}