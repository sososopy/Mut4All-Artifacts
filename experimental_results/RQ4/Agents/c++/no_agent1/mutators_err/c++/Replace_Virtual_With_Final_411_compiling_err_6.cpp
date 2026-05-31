//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Virtual_With_Final_411
 */ 
class MutatorFrontendAction_411 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(411)
private:
    class MutatorASTConsumer_411 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_411(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/replace_virtual_with_final_411.h"

// ========================================================================================================
#define MUT411_OUTPUT 1

void MutatorFrontendAction_411::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
            return;

        if (MD->isVirtual() && !MD->hasAttr<clang::FinalAttr>()) {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                       MD->getSourceRange());
            size_t pos = declaration.find("virtual");
            if (pos != std::string::npos) {
                declaration.replace(pos, 7, "final");
            }
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MD->getSourceRange()), declaration);
        }
    }
}

void MutatorFrontendAction_411::MutatorASTConsumer_411::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isVirtual(), unless(hasAttr<clang::attr::Final>())).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}