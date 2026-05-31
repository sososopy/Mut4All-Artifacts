//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Constraints_357
 */ 
class MutatorFrontendAction_357 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(357)

private:
    class MutatorASTConsumer_357 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_357(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Constraints_357.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT357_OUTPUT 1

void MutatorFrontendAction_357::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VT = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("VarTemplate")) {
        if (!VT || !Result.Context->getSourceManager().isWrittenInMainFile(
                VT->getLocation()))
            return;

        auto templateParams = VT->getTemplateParameters();
        if (templateParams->size() != 1)
            return;

        if (auto *conceptDecl = dyn_cast<clang::ConceptDecl>(templateParams->getParam(0))) {
            std::string conceptName = conceptDecl->getNameAsString();
            std::string invalidConcept = "InvalidConcept";

            std::string modifiedCode = "template <typename T>\nconcept " + invalidConcept + " = false;\n";
            modifiedCode += "template <" + invalidConcept + " T>\n";
            modifiedCode += VT->getNameAsString() + ";\n";

            Rewrite.ReplaceText(VT->getSourceRange(), modifiedCode);
        }
    }
}

void MutatorFrontendAction_357::MutatorASTConsumer_357::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varTemplateDecl(has(templateTypeParmDecl())).bind("VarTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}