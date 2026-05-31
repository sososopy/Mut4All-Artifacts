//header file
#pragma once
#include "Mutator_base.h"

/**
 * incorrect_template_deduction_guide_586
 */ 
class MutatorFrontendAction_586 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(586)

private:
    class MutatorASTConsumer_586 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_586(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/incorrect_template_deduction_guide_586.h"

// ========================================================================================================
#define MUT586_OUTPUT 1

void MutatorFrontendAction_586::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;

      auto className = CTD->getNameAsString();
      std::string deductionGuide = className + "() " + className + " obj;";
      deductionGuide = "/*mut586*/" + deductionGuide;

      SourceLocation insertLoc = CTD->getEndLoc().getLocWithOffset(1);
      Rewrite.InsertText(insertLoc, deductionGuide, true, true);
    }
}
  
void MutatorFrontendAction_586::MutatorASTConsumer_586::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}