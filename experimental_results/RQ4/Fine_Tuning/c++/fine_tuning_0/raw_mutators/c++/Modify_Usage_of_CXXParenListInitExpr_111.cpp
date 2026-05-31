//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Usage_of_CXXParenListInitExpr_111
 */ 
class MutatorFrontendAction_111 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(111)

private:
    class MutatorASTConsumer_111 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_111(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Usage_of_CXXParenListInitExpr_111.h"

// ========================================================================================================
#define MUT111_OUTPUT 1

void MutatorFrontendAction_111::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXParenListInitExpr>("CXXParenListInitExpr")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      llvm::outs() << text << "\n";
      text = "/*mut111*/{" + text.substr(1, text.length() - 2) + "}";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), text);
    }
}
  
void MutatorFrontendAction_111::MutatorASTConsumer_111::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxParenListInitExpr().bind("CXXParenListInitExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}