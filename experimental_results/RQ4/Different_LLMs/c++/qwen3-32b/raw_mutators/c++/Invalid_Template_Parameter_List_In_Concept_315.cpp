//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Template_Parameter_List_In_Concept_315
 */ 
class MutatorFrontendAction_315 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(315)

private:
    class MutatorASTConsumer_315 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_315(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalid_Template_Parameter_List_In_Concept_315.h"

// ========================================================================================================
#define MUT315_OUTPUT 1

void MutatorFrontendAction_315::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("conceptDecl")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        auto *TPL = CD->getTemplateParameters();
        if (!TPL)
            return;
        SourceLocation start = TPL->getTemplateKeywordLoc();
        SourceLocation end = TPL->getRAngleLoc();
        if (start.isInvalid() || end.isInvalid())
            return;
        SourceManager &SM = Rewrite.getSourceMgr();
        LangOptions &LO = Rewrite.getLangOpts();
        std::string originalTplText = Lexer::getSourceText(CharSourceRange::getCharRange(start, end), SM, LO).str();
        std::string firstParam;
        size_t commaPos = originalTplText.find(',');
        if (commaPos == std::string::npos) {
            firstParam = originalTplText;
        } else {
            firstParam = originalTplText.substr(0, commaPos);
        }
        std::string newTplText = originalTplText + ", " + firstParam;
        Rewrite.ReplaceText(CharSourceRange::getCharRange(start, end), newTplText);
    }
}

void MutatorFrontendAction_315::MutatorASTConsumer_315::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("conceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}