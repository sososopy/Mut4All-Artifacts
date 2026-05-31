//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mismatched_Parameter_Pack_Syntax_302
 */ 
class MutatorFrontendAction_302 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(302)

private:
    class MutatorASTConsumer_302 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_302(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mismatched_Parameter_Pack_Syntax_302.h"

// ========================================================================================================
#define MUT302_OUTPUT 1

void MutatorFrontendAction_302::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *D = Result.Nodes.getNodeAs<clang::Decl>("PackParam")) {
        if (!D || !Result.Context->getSourceManager().isWrittenInMainFile(D->getLocation()))
            return;

        if (!D->isTemplateParameter() || !D->isParameterPack())
            return;

        SourceLocation startLoc = D->getBeginLoc();
        SourceLocation endLoc = D->getEndLoc();
        if (startLoc.isInvalid() || endLoc.isInvalid())
            return;

        std::string originalText = stringutils::rangetoStr(*Result.SourceManager, CharSourceRange::getTokenRange(startLoc, endLoc));

        size_t ellipsisPos = originalText.find("...");
        if (ellipsisPos == std::string::npos)
            return;

        std::string typePart = originalText.substr(0, ellipsisPos);
        typePart.erase(std::remove_if(typePart.begin(), typePart.end(), isspace), typePart.end());

        std::string identifierPart = originalText.substr(ellipsisPos + 3);
        identifierPart.erase(std::remove_if(identifierPart.begin(), identifierPart.end(), isspace), identifierPart.end());

        std::string mutatedText = typePart + " " + identifierPart + "...";

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(startLoc, endLoc), mutatedText);
    }
}

void MutatorFrontendAction_302::MutatorASTConsumer_302::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = decl(isTemplateParameter(), isParameterPack()).bind("PackParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}