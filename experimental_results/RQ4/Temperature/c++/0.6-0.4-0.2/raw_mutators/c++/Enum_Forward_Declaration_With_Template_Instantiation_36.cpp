```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * enum_forward_declaration_with_template_instantiation_36
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
    };
};

//source file
#include "../include/enum_forward_declaration_with_template_instantiation_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *EnumDecl = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        if (!EnumDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                EnumDecl->getLocation()))
            return;

        if (EnumDecl->isCompleteDefinition())
            return;

        if (auto *TemplateSpec = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpec")) {
            if (!TemplateSpec || !Result.Context->getSourceManager().isWrittenInMainFile(
                    TemplateSpec->getLocation()))
                return;

            auto staticAssertText = "static_assert(sizeof(" + EnumDecl->getNameAsString() + ") > 0, \"Incomplete type\");";
            Rewrite.InsertTextAfterToken(TemplateSpec->getEndLoc(), "\n/*mut36*/" + staticAssertText);
        }
    }
}

void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher enumMatcher = enumDecl(unless(isDefinition())).bind("EnumDecl");
    DeclarationMatcher templateMatcher = classTemplateSpecializationDecl(
        hasAnyTemplateArgument(refersToType(qualType(hasDeclaration(enumDecl()))))
    ).bind("TemplateSpec");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}
```