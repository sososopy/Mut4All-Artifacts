//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Instantiation_104
 */ 
class MutatorFrontendAction_104 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(104)

private:
    class MutatorASTConsumer_104 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_104(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Instantiation_104.h"

// ========================================================================================================
#define MUT104_OUTPUT 1

void MutatorFrontendAction_104::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunction")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      for (auto *Spec : MT->specializations()) {
        if (Spec->isExplicitSpecialization()) {
          auto specRange = Spec->getSourceRange();
          auto specText = stringutils::rangetoStr(*(Result.SourceManager), specRange);

          if (specText.find('<') != std::string::npos) {
            auto insertPos = specText.find('>');
            specText.insert(insertPos, ", char");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(specRange), specText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_104::MutatorASTConsumer_104::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}