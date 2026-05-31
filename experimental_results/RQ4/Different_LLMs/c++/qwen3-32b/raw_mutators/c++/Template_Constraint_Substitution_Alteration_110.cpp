//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Constraint_Substitution_Alteration_110
 */ 
class MutatorFrontendAction_Template_Constraint_Substitution_Alteration_110 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(110)

private:
    class MutatorASTConsumer_110 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_110(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Template_Constraint_Substitution_Alteration_110.h"

// ========================================================================================================
#define MUT110_OUTPUT 1

void MutatorFrontendAction_Template_Constraint_Substitution_Alteration_110::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("funcTemplate")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;
        SourceManager &SM = Result.Context->getSourceManager();
        Rewriter &Rewrite = this->Rewrite;
        const TemplateParameterList *TPL = FT->getTemplateParameters();
        if (!TPL)
            return;
        const RequiresExpr *RE = FT->getRequiresClause();
        if (!RE)
            return;
        SourceRange tplRange = TPL->getSourceRange();
        std::string tplText = stringutils::rangetoStr(SM, tplRange);
        std::string newParam = ", template<class...> class U";
        std::string newTplText = tplText + newParam;
        Rewrite.ReplaceText(tplRange, newTplText);
        SourceRange reRange = RE->getSourceRange();
        std::string reText = stringutils::rangetoStr(SM, reRange);
        size_t requiresPos = reText.find("requires");
        if (requiresPos == std::string::npos)
            return;
        std::string constraintPart = reText.substr(requiresPos + 8);
        constraintPart.erase(0, constraintPart.find_first_not_of(" \t\n\r\f\v"));
        std::string newConstraint = "(" + constraintPart + " && requires { typename U<T>; })";
        std::string newReText = "requires " + newConstraint;
        Rewrite.ReplaceText(reRange, newReText);
    }
}

void MutatorFrontendAction_Template_Constraint_Substitution_Alteration_110::MutatorASTConsumer_110::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasRequiresClause()).bind("funcTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}