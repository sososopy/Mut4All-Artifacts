//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Decltype_In_Generic_Lambda_246
 */ 
class MutatorFrontendAction_246 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(246)

private:
    class MutatorASTConsumer_246 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_246(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalid_Decltype_In_Generic_Lambda_246.h"

// ========================================================================================================
#define MUT246_OUTPUT 1

void MutatorFrontendAction_246::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("GenericLambda")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      auto lambda = stringutils::rangetoStr(*(Result.SourceManager),
                                            LE->getSourceRange());
      if (!LE->isGenericLambda())
        return;
      llvm::outs() << lambda << '\n';
      lambda.insert(lambda.find('[') + 1, "/*mut246*/decltype()");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambda);
    }
}
  
void MutatorFrontendAction_246::MutatorASTConsumer_246::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("GenericLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}