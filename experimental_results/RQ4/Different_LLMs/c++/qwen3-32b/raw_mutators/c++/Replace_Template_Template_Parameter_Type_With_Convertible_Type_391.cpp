//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Template_Parameter_Type_With_Convertible_Type_391
 */ 
class MutatorFrontendAction_391 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(391)

private:
    class MutatorASTConsumer_391 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_391(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateTypeParmDecl*> typeParams;
    };
};

//source file
#include "../include/Replace_Template_Template_Parameter_Type_With_Convertible_Type_391.h"

// ========================================================================================================
#define MUT391_OUTPUT 1

void MutatorFrontendAction_391::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *tparam = Result.Nodes.getNodeAs<clang::TemplateTemplateParmDecl>("tparam")) {
        if (!tparam || !Result.Context->getSourceManager().isWrittenInMainFile(tparam->getLocation()))
            return;

        const TemplateParameterList *params = tparam->getTemplateParameters();
        for (unsigned i = 0; i < params->size(); ++i) {
            const NamedDecl *param = params->getParam(i);
            if (const auto *tt = dyn_cast<clang::TemplateTypeParmDecl>(param)) {
                SourceLocation loc = tt->getLocation();
                if (loc.isInvalid())
                    continue;

                // Get the source range of the TemplateTypeParmDecl's name
                std::string name = tt->getNameAsString();
                SourceLocation start = loc;
                SourceLocation end = loc.getLocWithOffset(name.size() - 1);
                CharSourceRange range = CharSourceRange::getCharRange(SourceRange(start, end));

                if (!typeParams.empty()) {
                    const auto *replacement = typeParams[getrandom::getRandomIndex(typeParams.size())];
                    std::string replacementName = replacement->getNameAsString();
                    Rewrite.ReplaceText(range, replacementName);
                }
            }
        }
    } else if (auto *tp = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("typeParam")) {
        typeParams.push_back(tp);
    }
}

void MutatorFrontendAction_391::MutatorASTConsumer_391::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher1 = templateTemplateParmDecl().bind("tparam");
    DeclarationMatcher matcher2 = templateTypeParmDecl().bind("typeParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}