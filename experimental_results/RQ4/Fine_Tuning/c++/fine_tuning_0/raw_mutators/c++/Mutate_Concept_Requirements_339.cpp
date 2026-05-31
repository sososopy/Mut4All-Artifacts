//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Concept_Requirements_339
 */ 
class MutatorFrontendAction_339 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(339)

private:
    class MutatorASTConsumer_339 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_339(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutate_Concept_Requirements_339.h"

// ========================================================================================================
#define MUT339_OUTPUT 1

void MutatorFrontendAction_339::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto concept_name = MT->getNameAsString();
      auto *TL = MT->getTemplateParameters();
      if (TL->size() != 2)
        return;
      auto *TR = MT->getConstraintExpr();
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             TR->getSourceRange());
      auto pos1 = content.find("==");
      if (pos1 == string::npos)
        return;
      content.replace(pos1, 2, "+");
      llvm::outs() << content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TR->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_339::MutatorASTConsumer_339::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}