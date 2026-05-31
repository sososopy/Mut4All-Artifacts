//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Attributes_From_Declaration_173
 */ 
class MutatorFrontendAction_173 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(173)

private:
    class MutatorASTConsumer_173 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_173(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Attributes_From_Declaration_173.h"

// ========================================================================================================
#define MUT173_OUTPUT 1

void MutatorFrontendAction_173::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::Decl>("AttrDecl")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        auto attrs = MT->getAttrs();
        if (attrs.empty())
            return;
        SourceManager &SM = *Result.SourceManager;
        SourceRange attrRange = attrs.front()->getSourceRange();
        for (size_t i = 1; i < attrs.size(); ++i) {
            attrRange.setEnd(attrs[i]->getSourceRange().getEnd());
        }
        CharSourceRange csr = CharSourceRange::getCharRange(attrRange);
        Rewrite.ReplaceText(csr, "");
    }
}
  
void MutatorFrontendAction_173::MutatorASTConsumer_173::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = (functionDecl(hasAttrs(anything())) || varDecl(hasAttrs(anything()))).bind("AttrDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}