//header file
#pragma once
#include "Mutator_base.h"

/**
 * Redundant_Constraint_On_Template_Template_Parameter_124
 */ 
class MutatorFrontendAction_124 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(124)

private:
    class MutatorASTConsumer_124 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_124(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Redundant_Constraint_On_Template_Template_Parameter_124.h"

// ========================================================================================================
#define MUT124_OUTPUT 1

void MutatorFrontendAction_124::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;

        TemplateParameterList *TPL = FTD->getTemplateParameters();
        if (!TPL)
            return;

        std::string tName;
        std::string nName;

        for (const auto *param : TPL->asArray()) {
            if (auto *TTP = clang::dyn_cast<clang::TemplateTemplateParmDecl>(param)) {
                tName = TTP->getNameAsString();
            } else if (auto *NTTP = clang::dyn_cast<clang::NonTypeTemplateParmDecl>(param)) {
                if (NTTP->getType()->isIntegralOrUnscopedEnumerationType()) {
                    nName = NTTP->getNameAsString();
                }
            }
        }

        if (tName.empty() || nName.empty())
            return;

        std::string requiresClause = " requires std::is_same_v<decltype(std::declval<" + tName + "<int, " + nName + ">>().x), int>";
        SourceLocation endLoc = TPL->getSourceRange().getEnd();
        Rewrite.InsertText(endLoc, requiresClause, true, true);
    }
}

void MutatorFrontendAction_124::MutatorASTConsumer_124::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}