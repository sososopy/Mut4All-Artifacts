//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decltype_With_Type_Alias_126
 */ 
class MutatorFrontendAction_126 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(126)

private:
    class MutatorASTConsumer_126 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_126(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_decltype_with_type_alias_126.h"

// ========================================================================================================
#define MUT126_OUTPUT 1

void MutatorFrontendAction_126::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAlias")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;

      auto target =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (target.find("decltype") == string::npos)
        return;
      if (DL->getUnderlyingType()->isFunctionPointerType() ||
          DL->getUnderlyingType()->isFunctionReferenceType() ||
          DL->getUnderlyingType()->isFunctionType()) {
        return;
      }
      auto realtype =
          DL->getUnderlyingType().getNonReferenceType().getAsString();
      llvm::outs() << realtype << '\n';
      llvm::outs() << target << '\n';
      stringutils::strReplaceAll(target, "decltype(auto)", realtype);
      target = "/*mut126*/" + target;
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), target);
    }
}
  
void MutatorFrontendAction_126::MutatorASTConsumer_126::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasDecl().bind("TypeAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}