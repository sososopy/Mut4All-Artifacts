//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * enforce_enum_type_check_36
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
        const EnumDecl *ForwardDeclaredEnum = nullptr;
    };
};

//source file
#include "../include/enforce_enum_type_check_36.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(ED->getLocation()))
            return;

        if (ED->isCompleteDefinition()) {
            return;
        }

        ForwardDeclaredEnum = ED;
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("templateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (!ForwardDeclaredEnum)
            return;

        std::string enforceTypeCheck = "\n// Enforce complete type\n";
        enforceTypeCheck += "template <class T>\nvoid enforce_complete_type() {\n";
        enforceTypeCheck += "    static_assert(sizeof(T) > 0, \"Incomplete type\");\n";
        enforceTypeCheck += "}\nenforce_complete_type<";
        enforceTypeCheck += ForwardDeclaredEnum->getNameAsString();
        enforceTypeCheck += ">();\n";

        SourceLocation insertLocation = TD->getEndLoc();
        Rewrite.InsertTextAfterToken(insertLocation, enforceTypeCheck);
    }
}

void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher enumMatcher = enumDecl(unless(isDefinition())).bind("enumDecl");
    DeclarationMatcher templateMatcher = templateDecl().bind("templateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}