//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_argument_type_mismatch_45
 */ 
class MutatorFrontendAction_45 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(45)

private:
    class MutatorASTConsumer_45 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_45(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const clang::TemplateDecl*, std::string> templateTypes;
    };
};

//source file
#include "../include/template_argument_type_mismatch_45.h"

// ========================================================================================================
#define MUT45_OUTPUT 1

void MutatorFrontendAction_45::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TI = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateInst")) {
        if (!TI)
            return;
        
        const TemplateDecl *templateDecl = TI->getTemplateName().getAsTemplateDecl();
        if (!templateDecl)
            return;

        if (templateTypes.find(templateDecl) == templateTypes.end()) {
            std::string newTypeName = "MutatedType_" + std::to_string(templateTypes.size());
            // Insert a new type definition in the main file
            std::string newTypeDef = "\nstruct " + newTypeName + " {};\n";
            Rewrite.InsertTextBefore(Result.SourceManager->getExpansionLoc(templateDecl->getLocation()), "/*mut45*/" + newTypeDef);
            templateTypes[templateDecl] = newTypeName;
        }

        std::string originalTypeName = TI->getArg(0).getAsType().getAsString();
        std::string mutatedTypeName = templateTypes[templateDecl];
        std::string newInstantiation = TI->getTemplateName().getAsTemplateDecl()->getNameAsString() +
                                       "<" + mutatedTypeName + ">";
        
        if (const auto *TST = TI->getAsType()) {
            Rewrite.ReplaceText(TST->getTypeLoc().getSourceRange(), newInstantiation);
        }
    }
}

void MutatorFrontendAction_45::MutatorASTConsumer_45::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = templateSpecializationType().bind("TemplateInst");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}