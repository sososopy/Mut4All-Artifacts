//header file
#pragma once
#include "Mutator_base.h"

/**
 * Self_referential_Concept_209
 */ 
class MutatorFrontendAction_209 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(209)

private:
    class MutatorASTConsumer_209 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_209(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Self_referential_Concept_209.h"

// ========================================================================================================
#define MUT209_OUTPUT 1

void MutatorFrontendAction_209::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      auto name = CD->getNameAsString();
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             CD->getSourceRange());
      content.insert(content.find('{') + 1, name + "<T>;");
      llvm::outs() << content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_209::MutatorASTConsumer_209::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}