//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Concept_Template_Parameter_88
 */ 
class MutatorFrontendAction_88 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(88)

private:
    class MutatorASTConsumer_88 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_88(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Nested_Concept_Template_Parameter_88.h"

// ========================================================================================================
#define MUT88_OUTPUT 1

void MutatorFrontendAction_88::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("OuterTemplate")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;

        clang::CXXRecordDecl *PrimaryTemplate = CT->getTemplatedDecl();
        if (!PrimaryTemplate || !PrimaryTemplate->isCompleteDefinition())
            return;

        bool hasConceptParam = false;
        for (unsigned i = 0, e = PrimaryTemplate->getNumTemplateParameters(); i < e; ++i) {
            const clang::TemplateParameter *Param = PrimaryTemplate->getTemplateParameter(i);
            if (const clang::TemplateTypeParmDecl *TTP = dyn_cast<clang::TemplateTypeParmDecl>(Param)) {
                if (TTP->hasRequiresClause()) {
                    hasConceptParam = true;
                    break;
                }
            }
        }

        if (!hasConceptParam)
            return;

        std::string innerClassCode = R"(
    template <typename U requires std::same_as<U, int>>
    class Inner {
    };
)";

        SourceLocation endLoc = PrimaryTemplate->getEndLoc();
        if (endLoc.isValid()) {
            Rewrite.InsertTextBefore(endLoc, innerClassCode);
        }
    }
}

void MutatorFrontendAction_88::MutatorASTConsumer_88::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(
        hasTemplatedDecl(cxxRecordDecl().bind("OuterTemplate"))
    ).bind("OuterTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}