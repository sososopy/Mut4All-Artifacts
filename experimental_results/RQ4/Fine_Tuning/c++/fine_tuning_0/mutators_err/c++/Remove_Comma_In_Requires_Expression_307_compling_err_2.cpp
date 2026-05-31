//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/ExprCXX.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * Remove_Comma_In_Requires_Expression_307
 */ 
class MutatorFrontendAction_307 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(307)

private:
    class MutatorASTConsumer_307 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_307(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Comma_In_Requires_Expression_307.h"

// ========================================================================================================
#define MUT307_OUTPUT 1

void MutatorFrontendAction_307::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::RequiresExpr>("requiresExpr")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      auto localrange = MT->getLocalRange();
      auto localrange_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                    localrange);
      llvm::outs() << localrange_str << '\n';
      auto pos = localrange_str.find(',');
      if (pos == string::npos)
        return;
      localrange_str.erase(pos, 1);
      content.replace(content.find('{'), content.rfind('}') - content.find('{'),
                      localrange_str);
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_307::MutatorASTConsumer_307::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = requiresExpr().bind("requiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}