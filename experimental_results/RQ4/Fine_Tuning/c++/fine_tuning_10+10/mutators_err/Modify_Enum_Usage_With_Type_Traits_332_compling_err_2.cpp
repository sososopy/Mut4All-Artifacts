//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Enum_Usage_With_Type_Traits_332
 */ 
class MutatorFrontendAction_332 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(332)

private:
    class MutatorASTConsumer_332 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_332(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Enum_Usage_With_Type_Traits_332.h"
#include <vector>

// ========================================================================================================
#define MUT332_OUTPUT 1

void MutatorFrontendAction_332::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(ED->getLocation()))
            return;

        for (auto *EnumConstant : ED->enumerators()) {
            if (EnumConstant) {
                std::string enumName = ED->getNameAsString();
                std::string constName = EnumConstant->getNameAsString();
                std::string replacement = "std::integral_constant<" + enumName + ", " + constName + ">";

                std::vector<const clang::DeclRefExpr*> uses;
                for (auto *Use : Result.Nodes.getNodeAs<std::vector<const clang::DeclRefExpr*>>("EnumUse")) {
                    uses.push_back(Use);
                }

                for (auto *Use : uses) {
                    if (Use && Use->getDecl() == EnumConstant) {
                        Rewrite.ReplaceText(Use->getSourceRange(), replacement);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_332::MutatorASTConsumer_332::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher enumMatcher = enumDecl().bind("EnumDecl");
    StatementMatcher useMatcher = declRefExpr(to(enumConstantDecl())).bind("EnumUse");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.addMatcher(useMatcher, &callback);
    matchFinder.matchAST(Context);
}