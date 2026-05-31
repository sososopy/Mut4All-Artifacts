//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Template_Parameter_To_Lambda_In_Concept_64
 */ 
class MutatorFrontendAction_64 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(64)

private:
    class MutatorASTConsumer_64 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_64(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Template_Parameter_To_Lambda_In_Concept_64.h"

// ========================================================================================================
#define MUT64_OUTPUT 1

void MutatorFrontendAction_64::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;

      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             LE->getSourceRange());
      llvm::outs() << content << '\n';
      auto pos = content.find('[');
      if (pos != string::npos) {
        content.insert(pos + 1, "/*mut64*/<typename> ");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()),
                            content);
      }
    }
}
  
void MutatorFrontendAction_64::MutatorASTConsumer_64::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher =
        lambdaExpr(hasAncestor(declRefExpr(to(expr())))).bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}