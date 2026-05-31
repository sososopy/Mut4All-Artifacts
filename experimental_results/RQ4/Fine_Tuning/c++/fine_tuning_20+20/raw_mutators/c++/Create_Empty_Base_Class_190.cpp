//header file
#pragma once
#include "Mutator_base.h"

/**
 * Create_Empty_Base_Class_190
 */ 
class MutatorFrontendAction_190 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(190)
    
    private:
      class MutatorASTConsumer_190 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_190(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<const clang::CXXRecordDecl *> base_classes;
      };
    };

//source file
#include "../include/create_empty_base_class_190.h"

// ========================================================================================================
#define MUT190_OUTPUT 1

void MutatorFrontendAction_190::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    for (std::size_t j = 0; j < base_classes.size(); ++j) {
      if (DL->isDerivedFrom(base_classes[j])) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               base_classes[j]->getSourceRange());
        if (content.rfind('}') != string::npos)
          content.insert(content.rfind('}'), "/*mut190*/");
        llvm::outs() << content;
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(base_classes[j]->getSourceRange()),
            content);
      }
    }
  } else if (auto *BL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Base")) {
    if (!BL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   BL->getLocation()))
      return;
    if ((!BL->isStruct() && !BL->isClass()) || BL->isLambda())
      return;
    base_classes.push_back(BL);
  }
}

void MutatorFrontendAction_190::MutatorASTConsumer_190::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto base_matcher = cxxRecordDecl().bind("Base");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(base_matcher, &callback);
    matchFinder.matchAST(Context);
}