//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Deduction_Guide_445
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
    };
};

//source file
#include "../include/Modify_Template_Deduction_Guide_445.h"

// ========================================================================================================
#define MUT445_OUTPUT 1

void MutatorFrontendAction_445::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TDG = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("TemplateGuide")) {
      if (!TDG || !Result.Context->getSourceManager().isWrittenInMainFile(TDG->getLocation()))
        return;

      auto deductionGuide = stringutils::rangetoStr(*(Result.SourceManager), TDG->getSourceRange());
      
      // Perform mutation by altering template parameter order
      size_t arrowPos = deductionGuide.find("->");
      if (arrowPos != std::string::npos) {
          std::string beforeArrow = deductionGuide.substr(0, arrowPos);
          std::string afterArrow = deductionGuide.substr(arrowPos);
          
          // Swap template parameters in the deduction guide
          std::string mutatedGuide = beforeArrow + "->" + "bar<U, T>;";
          
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(TDG->getSourceRange()), mutatedGuide);
      }
    }
}
  
void MutatorFrontendAction_445::MutatorASTConsumer_445::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("TemplateGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}