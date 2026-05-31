//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misuse_Template_Instantiation_310
 */ 
class MutatorFrontendAction_310 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(310)

private:
    class MutatorASTConsumer_310 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_310(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misuse_template_instantiation_310.h"

// ========================================================================================================
#define MUT310_OUTPUT 1

void MutatorFrontendAction_310::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        auto typeLoc = VD->getTypeSourceInfo()->getTypeLoc();
        if (auto templateSpecTypeLoc = typeLoc.getAs<clang::TemplateSpecializationTypeLoc>()) {
            auto templateName = templateSpecTypeLoc.getTypePtr()->getTemplateName().getAsTemplateDecl()->getNameAsString();
            auto sourceRange = templateSpecTypeLoc.getSourceRange();
            std::string replacement = "std::" + templateName + " ";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), replacement);
        }
    }
}

void MutatorFrontendAction_310::MutatorASTConsumer_310::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(templateSpecializationType())).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}