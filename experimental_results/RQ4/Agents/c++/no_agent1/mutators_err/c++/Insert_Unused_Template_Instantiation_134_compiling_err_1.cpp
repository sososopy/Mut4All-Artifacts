//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Template_Instantiation_134
 */ 
class MutatorFrontendAction_134 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(134)

private:
    class MutatorASTConsumer_134 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_134(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> templateNames;
    };
};

//source file
#include "../include/insert_unused_template_instantiation_134.h"

// ========================================================================================================
#define MUT134_OUTPUT 1

void MutatorFrontendAction_134::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (TD->isThisDeclarationADefinition()) {
            std::string templateName = TD->getNameAsString();
            templateNames.insert(templateName);
        }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (!templateNames.empty()) {
            std::string randomTemplate = *std::next(templateNames.begin(), getrandom::getRandomIndex(templateNames.size()));
            std::string instantiation = "\n/*mut134*/ " + randomTemplate + "<int> unusedInstance;\n";
            Rewrite.InsertTextBefore(FD->getBody()->getBeginLoc(), instantiation);
        }
    }
}

void MutatorFrontendAction_134::MutatorASTConsumer_134::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("Templates");
    DeclarationMatcher functionMatcher = functionDecl(hasBody()).bind("Functions");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}