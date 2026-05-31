//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Requires_With_Template_Lambda_316
 */ 
class MutatorFrontendAction_316 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(316)

private:
    class MutatorASTConsumer_316 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_316(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Requires_With_Template_Lambda_316.h"

// ========================================================================================================
#define MUT316_OUTPUT 1

void MutatorFrontendAction_316::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (auto *Req = FD->getRequiresClause()) {
            SourceLocation startLoc = Req->getBeginLoc();
            SourceLocation endLoc = Req->getEndLoc();

            if (startLoc.isInvalid() || endLoc.isInvalid())
                return;

            auto *SM = Result.SourceManager;
            string reqText = stringutils::rangetoStr(*SM, CharSourceRange::getCharRange(startLoc, endLoc));

            size_t openBracePos = reqText.find('{');
            if (openBracePos == string::npos)
                return;

            size_t closeBracePos = reqText.rfind('}');
            if (closeBracePos == string::npos || closeBracePos <= openBracePos)
                return;

            string newBody = "[]<typename Y>(ExistingType<Y>& e) { e.f(); }(t);";
            string newReqText = reqText.substr(0, openBracePos + 1) + newBody + reqText.substr(closeBracePos);

            Rewrite.ReplaceText(CharSourceRange::getCharRange(startLoc, endLoc), newReqText);
        }
    }
}

void MutatorFrontendAction_316::MutatorASTConsumer_316::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasRequiresClause()).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}