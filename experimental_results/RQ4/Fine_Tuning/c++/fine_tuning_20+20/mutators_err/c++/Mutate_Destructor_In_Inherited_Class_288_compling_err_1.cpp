//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Destructor_In_Inherited_Class_288
 */ 
class MutatorFrontendAction_288 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(288)
    
    private:
      class MutatorASTConsumer_288 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_288(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<const clang::CXXRecordDecl *> cur_classes;
      };
    };

//source file
#include "../include/mutate_destructor_in_inherited_class_288.h"

// ========================================================================================================
#define MUT288_OUTPUT 1

void MutatorFrontendAction_288::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
      return;
    if (!CL->isCompleteDefinition())
      return;
    if (CL->hasAnyDependentBases())
      return;
    if (CL->isDerivedFrom(CL))
      return;
    if (CL->getNumBases() == 0 && !CL->hasAnyDependentBases() &&
        !CL->isPolymorphic())
      return;
    cur_classes.push_back(CL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                 "Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (!DL->isCompleteDefinition())
      return;
    if (DL->hasAnyDependentBases())
      return;
    if (DL->isDerivedFrom(DL))
      return;
    if (DL->getNumBases() == 0 && !DL->hasAnyDependentBases() &&
        !DL->isPolymorphic())
      return;
    auto TD = DL->getDestructor();
    if (TD == nullptr)
      return;
    auto def =
        stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
    if (TD->isImplicit()) {
      def = DL->getNameAsString();
      def = "~" + def + "()";
      def = "/*mut288*/" + def;
      Rewrite.ReplaceText(DL->getEndLoc(), 0, def);
    } else if (TD->isExplicitlyDefaulted() || TD->isExplicitlyDeleted()) {
      def = DL->getNameAsString();
      def = "~" + def + "()";
      def = "/*mut288*/" + def;
      Rewrite.ReplaceText(TD->getSourceRange(), def);
    } else {
      if (DL->isPolymorphic()) {
        if (TD->isVirtual()) {
          if (TD->hasAttr<OverrideAttr>()) {
            def = "virtual " + def;
          } else {
            def = "virtual " + def + " override";
          }
        } else {
          def = "virtual " + def;
        }
      }
      def += " : return 0";
      llvm::outs() << def << '\n';
      Rewrite.ReplaceText(TD->getSourceRange(), def);
    }
  }
}

void MutatorFrontendAction_288::MutatorASTConsumer_288::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.matchAST(Context);
}