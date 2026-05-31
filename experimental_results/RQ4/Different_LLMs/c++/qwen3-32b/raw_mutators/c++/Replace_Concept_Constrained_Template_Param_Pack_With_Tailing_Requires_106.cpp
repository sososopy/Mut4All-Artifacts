//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concept_Constrained_Template_Param_Pack_With_Tailing_Requires_106
 */ 

class MutatorFrontendAction_106 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(106)

private:
    class MutatorASTConsumer_106 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_106(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Concept_Constrained_Template_Param_Pack_With_Tailing_Requires_106.h"

// ========================================================================================================
#define MUT106_OUTPUT 1

void MutatorFrontendAction_106::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        auto *TPL = MT->getTemplateParameters();
        if (!TPL)
            return;

        for (auto *Param : TPL->asArray()) {
            if (auto *NTTP = dyn_cast<clang::NonTypeTemplateParmDecl>(Param)) {
                if (NTTP->isParameterPack() && NTTP->hasTypeConstraint()) {
                    auto *Concept = NTTP->getTypeConstraintConcept();
                    if (!Concept)
                        continue;
                    std::string ConceptName = Concept->getNameAsString();
                    std::string ParamName = NTTP->getNameAsString();

                    if (auto *ConstraintExpr = NTTP->getTypeConstraintExpr()) {
                        SourceLocation Start = ConstraintExpr->getBeginLoc();
                        SourceLocation End = ConstraintExpr->getEndLoc();
                        if (Start.isValid() && End.isValid()) {
                            Rewrite.RemoveText(Start, End);
                        }
                    }

                    std::string RequiresClause = " requires (... && " + ConceptName + "<decltype(" + ParamName + ")>)";
                    SourceLocation InsertLoc = TPL->getSourceRange().getEnd();
                    Rewrite.InsertText(InsertLoc, RequiresClause, true, true);
                }
            }
        }
    }
}

void MutatorFrontendAction_106::MutatorASTConsumer_106::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
        isTemplate(),
        hasAncestor(cxxRecordDecl(isTemplate())),
        hasTemplateParameters(nonTypeTemplateParmDecl(isParameterPack(), hasTypeConstraint()))
    ).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}