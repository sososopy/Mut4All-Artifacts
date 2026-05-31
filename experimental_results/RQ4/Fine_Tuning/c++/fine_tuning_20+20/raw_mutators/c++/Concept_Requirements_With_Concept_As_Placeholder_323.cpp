//header file
#pragma once
#include "Mutator_base.h"

/**
 * Concept_Requirements_With_Concept_As_Placeholder_323
 */ 
class MutatorFrontendAction_323 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(323)

private:
    class MutatorASTConsumer_323 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_323(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/concept_requirements_with_concept_as_placeholder_323.h"

// ========================================================================================================
#define MUT323_OUTPUT 1

void MutatorFrontendAction_323::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concept")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      auto name = MT->getNameAsString();
      if (content.find("requires") == string::npos)
        return;
      if (content.find("typename") == string::npos)
        return;
      if (content.find(name) == string::npos)
        return;
      auto pos = content.find("typename");
      content.replace(pos, 8, name);
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_323::MutatorASTConsumer_323::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("Concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}