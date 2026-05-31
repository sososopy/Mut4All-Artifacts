//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Deduction_Guide_Removal_445
 */ 
class MutatorFrontendAction_445 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(445)

private:
    class MutatorASTConsumer_445 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_445(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_deduction_guide_removal_445.h"

// ========================================================================================================
#define MUT445_OUTPUT 1

void MutatorFrontendAction_445::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>("DeductionGuides")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto deduction_guide = stringutils::rangetoStr(*(Result.SourceManager),
                                                     MT->getSourceRange());
      llvm::outs() << deduction_guide << '\n';
      //Perform mutation on the source code text by applying string replacement
      deduction_guide = "/*mut445*/";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), deduction_guide);
    }
}
  
void MutatorFrontendAction_445::MutatorASTConsumer_445::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxDeductionGuideDecl().bind("DeductionGuides");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}