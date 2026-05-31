//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Constrained_Inner_Template_87
 */ 
class MutatorFrontendAction_87 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(87)

private:
    class MutatorASTConsumer_87 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_87(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Constrained_Inner_Template_87.h"

// ========================================================================================================
#define MUT87_OUTPUT 1

void MutatorFrontendAction_87::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *outerClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("outerClass")) {
        if (!outerClass || !Result.Context->getSourceManager().isWrittenInMainFile(outerClass->getLocation()))
            return;
        if (!outerClass->isCompleteDefinition())
            return;

        const TemplateParameterList *TPL = outerClass->getTemplateParameterList();
        if (!TPL || TPL->size() == 0)
            return;

        const TemplateTypeParmDecl *firstParam = dyn_cast<TemplateTypeParmDecl>(TPL->getParam(0));
        if (!firstParam)
            return;
        std::string outerParamName = firstParam->getNameAsString();

        std::string innerTemplate = R"(
    template <typename U>
    requires (requires { )" + outerParamName + R"(::value; } && U::value)
    struct Inner {
    };
)";

        SourceLocation endLoc = outerClass->getEndLoc();
        if (!endLoc.isInvalid()) {
            Rewrite.InsertText(endLoc, innerTemplate, true, true);
        }
    }
}

void MutatorFrontendAction_87::MutatorASTConsumer_87::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(hasTemplateParams(anything())).bind("outerClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}