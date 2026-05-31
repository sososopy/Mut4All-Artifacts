//header file
#pragma once
#include "Mutator_base.h"

/**
 * NestedTemplateAccessQualification_152
 */ 
class MutatorFrontendAction_152 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(152)

private:
    class MutatorASTConsumer_152 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_152(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/NestedTemplateAccessQualification_152.h"

// ========================================================================================================
#define MUT152_OUTPUT 1

void MutatorFrontendAction_152::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TID = Result.Nodes.getNodeAs<clang::TemplateIdExpr>("templateId")) {
        if (!TID || !Result.Context->getSourceManager().isWrittenInMainFile(TID->getLocation()))
            return;

        auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("functionTemplate");
        if (!FTD)
            return;

        const auto *TPL = FTD->getTemplateParameters();
        if (!TPL || TPL->size() < 1)
            return;

        std::vector<std::string> params;
        for (unsigned i = 0; i < TPL->size(); ++i) {
            if (auto *TTP = dyn_cast<clang::TemplateTypeParmDecl>(TPL->getParam(i))) {
                params.push_back(TTP->getNameAsString());
            }
        }

        if (params.empty())
            return;

        std::string p1 = params[0];
        std::string p2 = (params.size() >= 2) ? params[1] : p1;

        SourceLocation start = TID->getTemplateNameLoc();
        SourceLocation end = TID->getClosingAngleLoc();
        auto SM = Result.Context->getSourceManager();
        auto originalText = stringutils::rangetoStr(*SM, TID->getSourceRange());

        std::string newTemplateText = "C<" + p1 + ">::template B<" + p2 + ">";

        Rewrite.ReplaceText(TID->getSourceRange(), newTemplateText);
    }
}
  
void MutatorFrontendAction_152::MutatorASTConsumer_152::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        hasDescendant(
            templateIdExpr(
                hasParent(cxxDependentScopeMemberExpr()),
                isMemberOfTemplate()
            ).bind("templateId")
        )
    ).bind("functionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}