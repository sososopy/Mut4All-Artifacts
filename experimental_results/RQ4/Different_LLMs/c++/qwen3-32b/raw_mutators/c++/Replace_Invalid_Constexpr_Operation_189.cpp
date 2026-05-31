//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Invalid_Constexpr_Operation_189
 */ 
class MutatorFrontendAction_189 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(189)

private:
    class MutatorASTConsumer_189 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_189(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Invalid_Constexpr_Operation_189.h"

// ========================================================================================================
#define MUT189_OUTPUT 1

void MutatorFrontendAction_189::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->hasBody())
            return;
        const Stmt *Body = FD->getBody();
        if (!Body)
            return;
        auto BodyRange = Body->getSourceRange();
        auto BodyText = stringutils::rangetoStr(*Result.SourceManager, BodyRange);
        size_t pos = BodyText.find('*');
        if (pos == string::npos)
            pos = BodyText.find('+');
        if (pos != string::npos) {
            std::string mutated = BodyText;
            mutated.replace(pos, 1, "/ 0");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(BodyRange), mutated);
        }
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("var")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        const Expr *Init = VD->getInitializer();
        if (!Init)
            return;
        auto InitRange = Init->getSourceRange();
        auto InitText = stringutils::rangetoStr(*Result.SourceManager, InitRange);
        size_t pos = InitText.find('*');
        if (pos == string::npos)
            pos = InitText.find('+');
        if (pos != string::npos) {
            std::string mutated = InitText;
            mutated.replace(pos, 1, "/ 0");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(InitRange), mutated);
        }
    }
}

void MutatorFrontendAction_189::MutatorASTConsumer_189::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = 
        (functionDecl(isConstexpr(), hasDescendant(binaryOperator())).bind("func") ||
         varDecl(isConstexpr(), hasInitializer(ignoringImpCasts(binaryOperator()))).bind("var"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}