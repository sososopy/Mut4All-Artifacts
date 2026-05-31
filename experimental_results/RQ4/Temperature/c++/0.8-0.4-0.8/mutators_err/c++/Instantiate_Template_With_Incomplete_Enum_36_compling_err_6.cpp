//header file
#pragma once
#include "Mutator_base.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * instantiate_template_with_incomplete_enum_36
 */ 
class MutatorFrontendAction_36 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(36)

private:
    class MutatorASTConsumer_36 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_36(Rewriter &R) : TheRewriter(R) {}
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
        const clang::TemplateDecl *templateDecl = nullptr;
    };
};

//source file
#include "../include/instantiate_template_with_incomplete_enum_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                TD->getLocation()))
            return;

        templateDecl = TD;
    } else if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                ED->getLocation()))
            return;

        if (templateDecl) {
            std::string templateName = templateDecl->getNameAsString();
            std::string enumName = ED->getNameAsString();
            std::string mutation = "static_assert(" + templateName + "<" + enumName + ">::value, \"Expecting a complete type.\");\n/*mut36*/";
            Rewrite.InsertTextAfterToken(ED->getEndLoc(), mutation);
        }
    }
}

void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;

    MatchFinder matchFinder;
    DeclarationMatcher templateMatcher = clang::ast_matchers::templateDecl().bind("TemplateDecl");
    DeclarationMatcher enumMatcher = clang::ast_matchers::enumDecl(unless(isDefinition())).bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.matchAST(Context);
}