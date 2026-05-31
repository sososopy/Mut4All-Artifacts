//header file
#pragma once
#include "Mutator_base.h"

/**
 * explicit_alias_template_instantiation_555
 */ 
class MutatorFrontendAction_555 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(555)

private:
    class MutatorASTConsumer_555 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_555(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_555.h"

// ========================================================================================================
#define MUT555_OUTPUT 1

void MutatorFrontendAction_555::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UTD = Result.Nodes.getNodeAs<clang::UsingDecl>("AliasTemplate")) {
        if (!UTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       UTD->getLocation()))
            return;

        if (auto *TT = UTD->getUnderlyingDecl()->getAs<clang::TypeAliasTemplateDecl>()) {
            auto *TAD = TT->getTemplatedDecl();
            auto *TD = TAD->getUnderlyingType().getTypePtr();

            if (auto *TST = dyn_cast<clang::TemplateSpecializationType>(TD)) {
                std::string replacementText = TAD->getNameAsString() + "<";

                for (unsigned i = 0; i < TST->getNumArgs(); ++i) {
                    if (i != 0) {
                        replacementText += ", ";
                    }
                    replacementText += TST->getArg(i).getAsType().getAsString();
                }
                replacementText += ">";

                auto sourceRange = UTD->getSourceRange();
                Rewrite.ReplaceText(sourceRange, replacementText);
            }
        }
    }
}

void MutatorFrontendAction_555::MutatorASTConsumer_555::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = usingDecl(hasType(typeAliasTemplateType())).bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}