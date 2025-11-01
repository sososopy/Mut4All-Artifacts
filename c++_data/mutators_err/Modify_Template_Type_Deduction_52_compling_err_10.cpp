// header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Type_Deduction_52
 */ 
class MutatorFrontendAction_52 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(52)

private:
    class MutatorASTConsumer_52 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_52(Rewriter &R) : TheRewriter(R) {}
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

// source file
#include "modify_template_type_deduction_52.h"

// ========================================================================================================
#define MUT52_OUTPUT 1

void MutatorFrontendAction_52::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAlias")) {
        if (!Result.Context->getSourceManager().isWrittenInMainFile(TA->getLocation()))
            return;

        auto aliasText = Result.SourceManager->getCharacterData(TA->getSourceRange().getBegin());
        std::string newAliasText = aliasText;
        size_t pos = newAliasText.find("= 0");
        if (pos != std::string::npos) {
            newAliasText.replace(pos, 3, "= 1");
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TA->getSourceRange()), newAliasText);
    }
}

void MutatorFrontendAction_52::MutatorASTConsumer_52::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasDecl(hasType(asString("SomeTemplate"))).bind("TypeAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}