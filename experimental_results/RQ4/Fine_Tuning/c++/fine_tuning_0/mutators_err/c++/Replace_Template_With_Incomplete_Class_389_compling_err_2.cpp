//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_With_Incomplete_Class_389
 */ 
class MutatorFrontendAction_389 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(389)

private:
    class MutatorASTConsumer_389 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_389(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_With_Incomplete_Class_389.h"

// ========================================================================================================
#define MUT389_OUTPUT 1

void MutatorFrontendAction_389::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      llvm::outs() << "Mutator 389: TemplateDecl found\n";
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << declaration << '\n';
      if (MT->isImplicit())
        return;
      if (isa<ClassTemplateDecl>(MT) == false)
        return;
      auto CT = cast<ClassTemplateDecl>(MT);
      auto TN = CT->getNameAsString();
      auto replacement = "class " + TN;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), replacement);
    }
}
  
void MutatorFrontendAction_389::MutatorASTConsumer_389::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = clang::ast_matchers::templateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}