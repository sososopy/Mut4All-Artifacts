//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Default_Template_Args_149
 */ 
class MutatorFrontendAction_149 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(149)

private:
    class MutatorASTConsumer_149 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_149(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Default_Template_Args_149.h"

// ========================================================================================================
#define MUT149_OUTPUT 1

void MutatorFrontendAction_149::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AliasDecl = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasDecl")) {
        if (!AliasDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                AliasDecl->getLocation()))
            return;

        const auto *TypeAlias = AliasDecl->getTemplatedDecl();
        if (!TypeAlias)
            return;

        const auto *TypeExpr = TypeAlias->getTypeSourceInfo()->getType().getTypePtrOrNull();
        if (!TypeExpr)
            return;

        auto DefaultArg = AliasDecl->getTemplateParameters()->getParam(0)->getDefaultArgument();
        if (DefaultArg.isNull())
            return;

        std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager),
                                                           DefaultArg->getSourceRange());
        std::string MutatedText = OriginalText + "*";

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(DefaultArg->getSourceRange()), MutatedText);
    }
}

void MutatorFrontendAction_149::MutatorASTConsumer_149::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasTemplateDecl(has(typeAliasDecl(hasType(asString("point<3, T2>"))))).bind("AliasDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}