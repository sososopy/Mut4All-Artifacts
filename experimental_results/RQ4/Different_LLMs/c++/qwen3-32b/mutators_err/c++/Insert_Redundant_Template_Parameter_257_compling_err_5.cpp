//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Redundant_Template_Parameter_257
 */ 
class MutatorFrontendAction_257 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(257)

private:
    class MutatorASTConsumer_257 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_257(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Insert_Redundant_Template_Parameter_257.h"

// ========================================================================================================
#define MUT257_OUTPUT 1

void MutatorFrontendAction_257::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
        // Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        auto *TPL = TD->getTemplateParameters();
        if (!TPL || TPL->size() < 1)
            return;

        bool hasPack = false;
        for (const auto *Param : *TPL) {
            if (Param->isParameterPack()) {
                hasPack = true;
                break;
            }
        }
        if (!hasPack)
            return;

        const auto *FirstParam = TPL->getParam(0);
        if (!isa<TemplateTypeParmDecl>(FirstParam))
            return;

        SourceManager &SM = *Result.SourceManager;
        SourceRange TPLRange = TPL->getSourceRange();
        std::string tplStr = stringutils::rangetoStr(SM, TPLRange);

        size_t firstComma = tplStr.find(',');
        if (firstComma == std::string::npos)
            return;

        std::string newTplStr = tplStr.insert(firstComma + 1, ", " + FirstParam->getNameAsString());

        Rewrite.ReplaceText(TPLRange, newTplStr);
    }
}

void MutatorFrontendAction_257::MutatorASTConsumer_257::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    using templateParameters = clang::ast_matchers::templateParameters;
    using hasAnyParameterPack = clang::ast_matchers::hasAnyParameterPack;
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(has(templateParameters(hasAnyParameterPack()))).bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}