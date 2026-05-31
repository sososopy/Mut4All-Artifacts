//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Template_Parameter_With_Requires_60
 */ 
class MutatorFrontendAction_60 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(60)

private:
    class MutatorASTConsumer_60 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_60(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Template_Parameter_With_Requires_60.h"

// ========================================================================================================
#define MUT60_OUTPUT 1

void MutatorFrontendAction_60::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TT = Result.Nodes.getNodeAs<clang::TemplateTemplateParmDecl>("templateParm")) {
        if (!TT || !Result.Context->getSourceManager().isWrittenInMainFile(TT->getLocation()))
            return;

        auto *OuterTemplate = dyn_cast<clang::ClassTemplateDecl>(TT->getDeclContext());
        if (!OuterTemplate)
            return;

        std::string requiresClause;
        for (unsigned i = 0; i < TT->getNumTemplateParameters(); ++i) {
            auto *Param = TT->getTemplateParameter(i);
            auto *TTP = dyn_cast<clang::TemplateTypeParmDecl>(Param);
            if (!TTP || !TTP->getTypeConstraint())
                continue;

            auto constraintType = TTP->getTypeConstraintType();
            auto *TS = constraintType->getAs<clang::TemplateSpecializationType>();
            if (!TS)
                continue;

            std::string constraintName = TS->getTemplateName().getAsQualifiedNameAsString();
            if (!requiresClause.empty())
                requiresClause += " && ";
            requiresClause += constraintName + "<decltype(Args)>";
        }

        if (requiresClause.empty())
            return;

        std::string fullRequiresClause = " requires " + requiresClause + "...";

        SourceLocation outerStart = OuterTemplate->getBeginLoc();
        SourceLocation outerEnd = OuterTemplate->getEndLoc();
        if (outerStart.isInvalid() || outerEnd.isInvalid())
            return;

        std::string outerCode = stringutils::rangetoStr(*Result.SourceManager, OuterTemplate->getSourceRange());
        if (outerCode.empty())
            return;

        size_t pos = outerCode.find(">");
        if (pos == std::string::npos)
            return;

        std::string mutatedCode = outerCode.substr(0, pos + 1) + fullRequiresClause + outerCode.substr(pos + 1);
        Rewrite.ReplaceText(OuterTemplate->getSourceRange(), mutatedCode);

        for (unsigned i = 0; i < TT->getNumTemplateParameters(); ++i) {
            auto *Param = TT->getTemplateParameter(i);
            auto *TTP = dyn_cast<clang::TemplateTypeParmDecl>(Param);
            if (!TTP || !TTP->getTypeConstraint())
                continue;

            auto constraintRange = TTP->getTypeConstraint()->getSourceRange();
            if (constraintRange.isInvalid())
                continue;
            Rewrite.ReplaceText(constraintRange, "");
        }
    }
}

void MutatorFrontendAction_60::MutatorASTConsumer_60::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateTemplateParmDecl().bind("templateParm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}