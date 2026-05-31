//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Variant_Arg_With_Pack_Expansion_225
 */ 
class MutatorFrontendAction_225 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(225)

private:
    class MutatorASTConsumer_225 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_225(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Variant_Arg_With_Pack_Expansion_225.h"

// ========================================================================================================
#define MUT225_OUTPUT 1

void MutatorFrontendAction_225::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TATD = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("alias")) {
        if (!TATD || !Result.Context->getSourceManager().isWrittenInMainFile(TATD->getLocation()))
            return;

        auto *TAD = TATD->getTemplatedDecl();
        if (!TAD)
            return;

        auto *TSI = TAD->getTypeSourceInfo();
        if (!TSI)
            return;

        clang::TypeLoc TL = TSI->getTypeLoc();
        if (auto *TSTL = TL.getAs<clang::TemplateSpecializationTypeLoc>()) {
            const clang::TemplateArgumentListInfo &Args = TSTL->getTemplateArgs();
            if (Args.size() < 2)
                return;

            unsigned index = getrandom::getRandomIndex(Args.size());
            const clang::TemplateArgumentLoc &argLoc = Args.arguments()[index];
            SourceRange argRange = argLoc.getSourceRange();

            const clang::TemplateTypeParmDecl *TParam = nullptr;
            for (const auto *Param : TATD->getTemplateParameters()) {
                if (isa<clang::TemplateTypeParmDecl>(Param)) {
                    TParam = cast<clang::TemplateTypeParmDecl>(Param);
                    break;
                }
            }

            if (!TParam)
                return;

            std::string replacement = TParam->getNameAsString() + "...";
            Rewrite.ReplaceText(argRange, replacement);
        }
    }
}
  
void MutatorFrontendAction_225::MutatorASTConsumer_225::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasTemplateDecl(
        hasTemplatedDecl(
            typeAliasDecl(
                hasType(
                    templateSpecializationType(
                        hasName("variant"),
                        hasAtLeastNumArgs(2)
                    )
                )
            )
        )
    ).bind("alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}