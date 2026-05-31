//header file
#pragma once
#include "Mutator_base.h"

/**
 * CTAD_Constrained_Default_TypeAlias_52
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
        bool conceptInserted = false;
    };
};

//source file
#include "../include/CTAD_Constrained_Default_TypeAlias_52.h"

// ========================================================================================================
#define MUT52_OUTPUT 1

void MutatorFrontendAction_52::Callback::run(const MatchFinder::MatchResult &Result) {
    SourceManager &SM = Rewrite.getSourceMgr();
    if (auto *classTemplate = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classTemplate")) {
        if (!classTemplate || !Result.Context->getSourceManager().isWrittenInMainFile(classTemplate->getLocation()))
            return;
        if (!classTemplate->isTemplateInstantiation())
            return;
        if (!conceptInserted) {
            SourceLocation startLoc = SM.getLocForStartOfFile(SM.getMainFileID());
            Rewrite.InsertText(startLoc, "template <class, class> concept C = true;\n", true, true);
            conceptInserted = true;
        }
        if (auto *constructor = classTemplate->getDeclaredConstructor()) {
            for (const auto *param : constructor->parameters()) {
                SourceRange typeRange = param->getTypeSourceInfo()->getTypeLoc().getSourceRange();
                Rewrite.ReplaceText(typeRange, "C<T> auto");
            }
        }
    } else if (auto *typeAlias = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("typeAlias")) {
        if (!typeAlias || !Result.Context->getSourceManager().isWrittenInMainFile(typeAlias->getLocation()))
            return;
        auto *typeAliasDecl = typeAlias->getTypeAliasDecl();
        for (const auto *param : typeAliasDecl->getTemplateParameters()) {
            if (param->hasDefaultArgument())
                continue;
            SourceRange paramRange = param->getSourceRange();
            std::string newParam = "class T = int";
            Rewrite.ReplaceText(paramRange, newParam);
            break;
        }
    }
}

void MutatorFrontendAction_52::MutatorASTConsumer_52::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher classMatcher = cxxRecordDecl(
        isTemplateInstantiation(),
        has(cxxConstructorDecl(
            unless(hasRequiresClause()),
            unless(isExplicitlyDefaulted()),
            unless(isDeleted())
        ).bind("constructor"))
    ).bind("classTemplate");
    DeclarationMatcher typeAliasMatcher = typeAliasTemplateDecl(
        hasTypeAliasDecl(
            hasTemplateParameters(
                anyOf(
                    has(parmThatRefersToType(templateTypeParmDecl(hasNoDefaultArgument()))),
                    has(parmThatRefersToType(templateTypeParmDecl(hasDefaultArgument())))
                )
            )
        )
    ).bind("typeAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(typeAliasMatcher, &callback);
    matchFinder.matchAST(Context);
}