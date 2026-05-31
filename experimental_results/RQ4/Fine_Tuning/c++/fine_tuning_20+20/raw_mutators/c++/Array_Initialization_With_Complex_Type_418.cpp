//header file
#pragma once
#include "Mutator_base.h"

/**
 * Array_Initialization_With_Complex_Type_418
 */ 
class MutatorFrontendAction_418 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(418)
    
    private:
      class MutatorASTConsumer_418 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_418(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/array_initialization_with_complex_type_418.h"

// ========================================================================================================
#define MUT418_OUTPUT 1

void MutatorFrontendAction_418::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("ArrayDecl")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->hasInit() == false)
      return;
    auto type = DL->getType();
    if (type->isArrayType() == false)
      return;
    auto arraytype = type->getAsArrayTypeUnsafe();
    if (arraytype == nullptr)
      return;
    auto elemtype = arraytype->getElementType();
    if (elemtype->isStructureOrClassType() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (content.rfind(';') != string::npos)
      content.insert(content.rfind(';'), "/*mut418*/={}");
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                        content);
  }
}

void MutatorFrontendAction_418::MutatorASTConsumer_418::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl().bind("ArrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}