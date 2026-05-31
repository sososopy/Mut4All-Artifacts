//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Derived_Constraint_With_Template_136
 */ 
class MutatorFrontendAction_136 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(136)
private:
    class MutatorASTConsumer_136 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_136(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Derived_Constraint_With_Template_136.h"

// ========================================================================================================
#define MUT136_OUTPUT 1

void MutatorFrontendAction_136::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TT = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("templateParam")) {
        if (!TT || !Result.Context->getSourceManager().isWrittenInMainFile(TT->getLocation()))
            return;

        auto *ClassTemplate = dyn_cast<clang::ClassTemplateDecl>(TT->getDeclContext()->getTemplateDecl());
        if (!ClassTemplate)
            return;

        clang::TemplateParameterList *Params = ClassTemplate->getTemplateParameters();
        const clang::TemplateTypeParmDecl *TParam = nullptr;
        for (unsigned i = 0; i < Params->size(); ++i) {
            if (auto *Param = dyn_cast<clang::TemplateTypeParmDecl>(Params->getParam(i))) {
                if (Param->getNameAsString() == "T") {
                    TParam = Param;
                    break;
                }
            }
        }

        if (!TParam)
            return;

        clang::RequiresClause *RC = TT->getRequiresClause();
        if (!RC)
            return;

        SourceLocation Start = RC->getBeginLoc();
        SourceLocation End = RC->getEndLoc();
        if (Start.isInvalid() || End.isInvalid())
            return;

        std::string OriginalText = stringutils::rangetoStr(*Result.SourceManager, CharSourceRange::getTokenRange(Start, End));
        std::string NewText = OriginalText;
        size_t Pos = NewText.find("Base");
        if (Pos != std::string::npos) {
            NewText.replace(Pos, 4, "Base<" + TParam->getNameAsString() + ", " + TT->getNameAsString() + ">");
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Start, End), NewText);
    }
}

void MutatorFrontendAction_136::MutatorASTConsumer_136::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateTypeParmDecl(
        inClassTemplate(),
        hasRequiresClause(
            requiresClause(
                hasCondition(declRefExpr(to(functionDecl(hasName("std::derived_from")))))
            )
        )
    ).bind("templateParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}