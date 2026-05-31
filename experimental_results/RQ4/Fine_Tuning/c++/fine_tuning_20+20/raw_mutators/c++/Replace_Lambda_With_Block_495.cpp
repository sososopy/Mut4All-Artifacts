//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_With_Block_495
 */ 
class MutatorFrontendAction_495 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(495)

private:
    class MutatorASTConsumer_495 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_495(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> cur_vars;
    };
};

//source file
#include "../include/Replace_Lambda_With_Block_495.h"

// ========================================================================================================
#define MUT495_OUTPUT 1

void MutatorFrontendAction_495::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LD = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!LD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LD->getBeginLoc()))
        return;
      if (LD->isMutable())
        return;
      auto captures = LD->captures();
      bool has_nonstatic_local = false;
      for (auto capture : captures) {
        if (capture.capturesVariable() &&
            capture.getCapturedVar()->isStaticLocal() == false) {
          has_nonstatic_local = true;
        }
      }
      if (has_nonstatic_local == false)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             LD->getSourceRange());
      llvm::outs() << content << '\n';
      content[content.find('[')] = '^';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LD->getSourceRange()),
                          content);
    } else if (auto *VD =
                   Result.Nodes.getNodeAs<clang::VarDecl>("LocalVars")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getBeginLoc()))
        return;
      if (VD->isStaticLocal())
        return;
      llvm::outs() << VD->getNameAsString() << '\n';
      cur_vars.push_back(VD);
    }
}

void MutatorFrontendAction_495::MutatorASTConsumer_495::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambda_matcher = lambdaExpr().bind("Lambdas");
    auto localvar_matcher = varDecl(hasAncestor(functionDecl())).bind("LocalVars");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.addMatcher(localvar_matcher, &callback);
    matchFinder.matchAST(Context);
}