//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Enum_Class_With_Using_Enum_80
 */ 
class MutatorFrontendAction_80 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(80)
    
    private:
      class MutatorASTConsumer_80 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_80(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_enum_class_with_using_enum_80.h"

// ========================================================================================================
#define MUT80_OUTPUT 1

void MutatorFrontendAction_80::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
    if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                   ED->getLocation()))
      return;
    if (ED->isScopedUsingClassTag() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), ED->getSourceRange());
    content = "using enum " + content;
    if (content.find("class") != string::npos)
      content.replace(content.find("class"), 5, "");
    if (content.find("enum") != string::npos)
      content.replace(content.find("enum"), 4, "");
    if (content.find('{') != string::npos)
      content.replace(content.find('{'), 1, ";");
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), content);
  }
}

void MutatorFrontendAction_80::MutatorASTConsumer_80::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = enumDecl().bind("Enums");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}