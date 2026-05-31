//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Enable_If_Condition_158
 */ 
class MutatorFrontendAction_158 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(158)

private:
    class MutatorASTConsumer_158 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_158(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Enable_If_Condition_158.h"

// ========================================================================================================
#define MUT158_OUTPUT 1

void MutatorFrontendAction_158::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TL = Result.Nodes.getNodeAs<clang::TypeLoc>("enableIfType")) {
        if (!TL || !Result.Context->getSourceManager().isWrittenInMainFile(TL->getBeginLoc()))
            return;

        const clang::TemplateSpecializationType *TST = TL.getType()->getAs<clang::TemplateSpecializationType>();
        if (!TST || TST->getTemplateName().getAsName() != "enable_if")
            return;

        const clang::TemplateArgument &CondArg = TST->getArg(0);
        if (CondArg.getKind() != clang::TemplateArgument::Expression)
            return;

        const clang::Expr *CondExpr = CondArg.getAsExpr();
        if (!CondExpr)
            return;

        clang::SourceRange condRange = CondExpr->getSourceRange();
        std::string conditionText = stringutils::rangetoStr(*Result.SourceManager, condRange);

        size_t colonPos = conditionText.find("::");
        if (colonPos != std::string::npos) {
            std::string base = conditionText.substr(0, colonPos);
            size_t lt = base.find('<');
            size_t gt = base.find('>');
            if (lt != std::string::npos && gt != std::string::npos) {
                std::string templateArg = base.substr(lt + 1, gt - lt - 1);
                std::string newCondition = templateArg + "::missing_member";
                Rewrite.ReplaceText(condRange, newCondition);
            }
        }
    }
}
  
void MutatorFrontendAction_158::MutatorASTConsumer_158::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeLoc(templateSpecializationType(hasName("enable_if"))).bind("enableIfType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}