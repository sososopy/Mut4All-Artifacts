//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Explicit_Specialization_For_Derived_Ctor_On_Base_Class_4
 */ 
class MutatorFrontendAction_4 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(4)

private:
    class MutatorASTConsumer_4 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_4(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> derived_classes;
    };
};

//source file
#include "../include/add_explicit_specialization_for_derived_ctor_on_base_class_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      derived_classes.push_back(DL);
    } else if (auto *BL =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Base")) {
      if (!BL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BL->getLocation()))
        return;
      if (BL->isCompleteDefinition() == false)
        return;
      llvm::outs() << BL->getNameAsString() << '\n';
      auto ctors = BL->ctors();
      for (auto ctor : ctors) {
        if (ctor->getDescribedFunctionTemplate() != nullptr) {
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 ctor->getSourceRange());
          for (auto derived : derived_classes) {
            content += "\n/*mut4*/template<>\n";
            content += BL->getNameAsString() + "::" +
                       ctor->getNameAsString() + "(";
            content += derived->getNameAsString() + "){}\n";
          }
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(ctor->getSourceRange()), content);
        }
      }
    }
}
  
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto base_matcher = cxxRecordDecl().bind("Base");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(base_matcher, &callback);
    matchFinder.matchAST(Context);
}