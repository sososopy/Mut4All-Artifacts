//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Malformed_Destructor_378
 */ 
class MutatorFrontendAction_378 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(378)
private:
    class MutatorASTConsumer_378 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_378(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_malformed_destructor_378.h"

// ========================================================================================================
#define MUT378_OUTPUT 1

void MutatorFrontendAction_378::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DT = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Dtor")) {
        if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(DT->getLocation()))
            return;
        if (!DT->isDefaulted())
            return;
        auto SM = Result.Context->getSourceManager();
        auto Loc = DT->getLocation();
        if (SM->isInSystemHeader(Loc))
            return;
        auto Range = DT->getSourceRange();
        auto OriginalText = stringutils::rangetoStr(*SM, Range);
        std::string MutatedText = OriginalText;
        size_t Pos = MutatedText.find("= default;");
        if (Pos != std::string::npos)
            MutatedText.replace(Pos, 10, "= {};");
        Rewrite.ReplaceText(Range, MutatedText);
    }
}
  
void MutatorFrontendAction_378::MutatorASTConsumer_378::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxDestructorDecl(isDefaulted()).bind("Dtor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}