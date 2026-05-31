//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Auto_Return_With_Pack_Indexing_204
 */ 
class MutatorFrontendAction_204 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(204)

private:
    class MutatorASTConsumer_204 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_204(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Auto_Return_With_Pack_Indexing_204.h"

// ========================================================================================================
#define MUT204_OUTPUT 1

void MutatorFrontendAction_204::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *func = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("func")) {
        if (!func || !Result.Context->getSourceManager().isWrittenInMainFile(func->getLocation()))
            return;

        if (!func->isStatic() || !func->isTemplate())
            return;

        QualType returnType = func->getReturnType();
        if (!returnType->isAutoType())
            return;

        const CXXRecordDecl *record = func->getParent();
        if (!record)
            return;

        TemplateParameterList *tparams = record->getTemplateParameters();
        if (!tparams)
            return;

        std::string packName;
        for (const auto &param : tparams->asArray()) {
            if (const auto *tp = dyn_cast<TemplateTypeParmDecl>(param)) {
                if (tp->isParameterPack()) {
                    packName = tp->getNameAsString();
                    break;
                }
            }
        }

        if (packName.empty())
            return;

        SourceRange returnTypeRange = func->getReturnTypeSourceRange();
        if (returnTypeRange.isInvalid())
            return;

        std::string originalReturnType = stringutils::rangetoStr(*Result.SourceManager, returnTypeRange);
        if (originalReturnType != "auto")
            return;

        std::string newReturnType = "std::tuple<" + packName + "...>";
        Rewrite.ReplaceText(returnTypeRange, newReturnType);
    }
}

void MutatorFrontendAction_204::MutatorASTConsumer_204::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
        isStatic(),
        isTemplate()
    ).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}