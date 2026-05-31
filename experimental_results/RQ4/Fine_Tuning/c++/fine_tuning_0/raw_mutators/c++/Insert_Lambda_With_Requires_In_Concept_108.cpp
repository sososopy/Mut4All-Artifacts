//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Lambda_With_Requires_In_Concept_108
 */ 
class MutatorFrontendAction_108 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(108)

private:
    class MutatorASTConsumer_108 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_108(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Lambda_With_Requires_In_Concept_108.h"

// ========================================================================================================
#define MUT108_OUTPUT 1

void MutatorFrontendAction_108::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;

      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), CD->getSourceRange());
      llvm::outs() << content << '\n';
      auto TL = CD->getTemplateInstantiationPattern();
      if (TL == nullptr)
        return;
      auto params = TL->getTemplateParameters();
      if (params->size() != 1)
        return;
      auto T = params->getParam(0);
      auto Tname = stringutils::rangetoStr(*(Result.SourceManager),
                                           T->getSourceRange());
      llvm::outs() << Tname << '\n';
      auto lambda = "[](){return requires(" + Tname +
                    " t){{t}->std::same_as<" + Tname + ">;};}()";
      content.insert(content.find('=') + 1, lambda + "&&");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_108::MutatorASTConsumer_108::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}