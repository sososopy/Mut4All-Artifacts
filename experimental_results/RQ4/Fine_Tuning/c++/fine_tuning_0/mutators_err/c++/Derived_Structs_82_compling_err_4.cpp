//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Variant_Initialization_With_Base-Derived_Structs_82
 */ 
class MutatorFrontendAction_82 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(82)

private:
    class MutatorASTConsumer_82 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_82(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::CXXRecordDecl *> base_classes;
    };
};

//source file
#include "../include/Modify_Variant_Initialization_With_Base-Derived_Structs_82.h"

// ========================================================================================================
#define MUT82_OUTPUT 1

void MutatorFrontendAction_82::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Base")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    llvm::outs() << DL->getNameAsString() << '\n';
    base_classes.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                 "Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    for (std::size_t j = 0; j < base_classes.size(); ++j) {
      llvm::outs() << base_classes[j]->getNameAsString() << '\n';
      if (DL->isDerivedFrom(base_classes[j])) {
        auto base_name = base_classes[j]->getNameAsString();
        auto derived_name = DL->getNameAsString();
        auto variant_name = "std::variant<" + base_name + "," + derived_name +
                            "> v = " + base_name + "{};";
        llvm::outs() << variant_name << '\n';
        Rewrite.ReplaceText(DL->getEndLoc(), 0, "/*mut82*/" + variant_name);
      }
    }
  }
}

void MutatorFrontendAction_82::MutatorASTConsumer_82::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto base_matcher = cxxRecordDecl().bind("Base");
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(base_matcher, &callback);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.matchAST(Context);
}