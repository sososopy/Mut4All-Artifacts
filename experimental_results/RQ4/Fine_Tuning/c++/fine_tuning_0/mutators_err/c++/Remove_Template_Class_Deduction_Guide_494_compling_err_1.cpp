//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Template_Class_Deduction_Guide_494
 */ 
class MutatorFrontendAction_494 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(494)

private:
    class MutatorASTConsumer_494 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_494(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Remove_Template_Class_Deduction_Guide_494.h"

// ========================================================================================================
#define MUT494_OUTPUT 1

void MutatorFrontendAction_494::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>("DeductionGuide")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto deduction_guide = stringutils::rangetoStr(*(Result.SourceManager),
                                                     MT->getSourceRange());
      llvm::outs() << deduction_guide << '\n';
      deduction_guide = "/*mut494*/" + deduction_guide;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), deduction_guide);
    }
}
  
void MutatorFrontendAction_494::MutatorASTConsumer_494::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = deductionGuideDecl().bind("DeductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}