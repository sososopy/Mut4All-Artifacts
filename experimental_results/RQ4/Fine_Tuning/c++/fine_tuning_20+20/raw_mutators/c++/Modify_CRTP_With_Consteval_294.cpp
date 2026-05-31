//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_CRTP_With_Consteval_294
 */ 
class MutatorFrontendAction_294 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(294)

private:
    class MutatorASTConsumer_294 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_294(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_crtp_with_consteval_294.h"

// ========================================================================================================
#define MUT294_OUTPUT 1

void MutatorFrontendAction_294::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->isConstexpr()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               MT->getSourceRange());
        llvm::outs() << content << '\n';
        auto pos = content.find("static_cast<Derived>(*this)");
        if (pos != string::npos) {
          content.replace(pos, 26, "dynamic_cast<Derived*>(this)");
        } else {
          pos = content.find("static_cast<Derived*>(this)");
          if (pos != string::npos) {
            content.replace(pos, 26, "reinterpret_cast<Derived&>(*this)");
          }
        }
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(MT->getSourceRange()), content);
      }
    }
}
  
void MutatorFrontendAction_294::MutatorASTConsumer_294::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl().bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}