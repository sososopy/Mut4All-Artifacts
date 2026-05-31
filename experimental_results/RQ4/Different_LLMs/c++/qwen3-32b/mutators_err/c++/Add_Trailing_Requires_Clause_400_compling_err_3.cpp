//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Trailing_Requires_Clause_400
 */ 
class MutatorFrontendAction_400 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(400)

private:
    class MutatorASTConsumer_400 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_400(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Add_Trailing_Requires_Clause_400.h"

// ========================================================================================================
#define MUT400_OUTPUT 1

void MutatorFrontendAction_400::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("methodDecl")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        if (MT->hasTrailingRequiresClause())
            return;

        auto *recordDecl = clang::cast<clang::CXXRecordDecl>(MT->getDeclContext());
        if (!recordDecl)
            return;

        auto *templateParams = recordDecl->getTemplateParameterList();
        if (!templateParams || templateParams->size() == 0)
            return;

        auto *firstParam = templateParams->getParam(0);
        std::string paramName = firstParam->getNameAsString();
        std::string condition = "sizeof(" + paramName + ") > 0";
        std::string requiresClause = " requires (" + condition + ")";

        auto originalText = stringutils::rangetoStr(*Result.SourceManager, MT->getSourceRange());
        size_t semicolonPos = originalText.find(';');
        if (semicolonPos != std::string::npos) {
            originalText.insert(semicolonPos, requiresClause);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), originalText);
        }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (FD->hasTrailingRequiresClause())
            return;

        auto *recordDecl = clang::cast<clang::CXXRecordDecl>(FD->getDeclContext());
        if (!recordDecl)
            return;

        auto *templateParams = recordDecl->getTemplateParameterList();
        if (!templateParams || templateParams->size() == 0)
            return;

        auto *firstParam = templateParams->getParam(0);
        std::string paramName = firstParam->getNameAsString();
        std::string condition = "sizeof(" + paramName + ") > 0";
        std::string requiresClause = " requires (" + condition + ")";

        auto originalText = stringutils::rangetoStr(*Result.SourceManager, FD->getSourceRange());
        size_t bracePos = originalText.find('{');
        if (bracePos != std::string::npos) {
            originalText.insert(bracePos, requiresClause);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), originalText);
        }
    }
}

void MutatorFrontendAction_400::MutatorASTConsumer_400::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = 
        cxxMethodDecl(
            hasDeclContext(cxxRecordDecl(has(templateParameterList()))),
            unless(clang::ast_matchers::hasTrailingRequiresClause())
        ).bind("methodDecl") ||
        functionDecl(
            isDefinition(),
            hasDeclContext(cxxRecordDecl(has(templateParameterList()))),
            unless(clang::ast_matchers::hasTrailingRequiresClause())
        ).bind("functionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}