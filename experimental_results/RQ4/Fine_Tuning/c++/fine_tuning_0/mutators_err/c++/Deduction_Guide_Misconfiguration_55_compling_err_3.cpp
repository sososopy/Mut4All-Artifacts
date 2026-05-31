//header file
#pragma once
#include "Mutator_base.h"

/**
 * Deduction_Guide_Misconfiguration_55
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
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        const clang::CXXRecordDecl *target_class = nullptr;
    };
};

//source file
#include "../include/Deduction_Guide_Misconfiguration_55.h"

// ========================================================================================================
#define MUT55_OUTPUT 1

void MutatorFrontendAction_55::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
        return;
      if (!DL->isCompleteDefinition())
        return;
      if (DL->getDescribedClassTemplate() == nullptr)
        return;
      llvm::outs() << DL->getNameAsString() << '\n';
      target_class = DL;
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>(
                   "TranslationUnit")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (target_class != nullptr) {
        auto name = target_class->getNameAsString();
        auto type = getrandom::getRandomIndex(1);
        auto guide = name + "() -> " + name + "<" + type + ">;";
        guide = "/*mut55*/" + guide;
        llvm::outs() << guide << '\n';
        Rewrite.ReplaceText(DL->getEndLoc(), 0, guide);
      }
    }
}
  
void MutatorFrontendAction_55::MutatorASTConsumer_55::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto translationUnit_matcher = translationUnitDecl().bind("TranslationUnit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(translationUnit_matcher, &callback);
    matchFinder.matchAST(Context);
}