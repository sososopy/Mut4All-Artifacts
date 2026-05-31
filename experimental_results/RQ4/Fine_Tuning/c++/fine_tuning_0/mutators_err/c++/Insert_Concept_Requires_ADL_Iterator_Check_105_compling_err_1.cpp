//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Concept_Requires_ADL_Iterator_Check_105
 */ 
class MutatorFrontendAction_105 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(105)

private:
    class MutatorASTConsumer_105 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_105(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Concept_Requires_ADL_Iterator_Check_105.h"

// ========================================================================================================
#define MUT105_OUTPUT 1

void MutatorFrontendAction_105::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             CT->getSourceRange());
      llvm::outs() << content << '\n';
      content.insert(content.find('{') + 1,
                     "/*mut105*/[&]{using std::begin;using std::end;using "
                     "std::swap;return requires{typename "
                     "std::input_iterator<T>;};}()");
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CT->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_105::MutatorASTConsumer_105::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}