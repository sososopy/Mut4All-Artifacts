//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Dependent_Default_Argument_In_Template_Alias_407
 */ 
class MutatorFrontendAction_407 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(407)

private:
    class MutatorASTConsumer_407 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_407(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Introduce_Dependent_Default_Argument_In_Template_Alias_407.h"

// ========================================================================================================
#define MUT407_OUTPUT 1

void MutatorFrontendAction_407::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *alias = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("alias")) {
        if (!alias || !Result.Context->getSourceManager().isWrittenInMainFile(alias->getLocation()))
            return;

        const TemplateTypeParmDecl *nonDepParam = nullptr;
        const TemplateTypeParmDecl *existingParam = nullptr;

        for (const auto *param : alias->getTemplateParameters()->asArray()) {
            if (auto *ttParam = dyn_cast<TemplateTypeParmDecl>(param)) {
                if (ttParam->hasDefaultArgument() && 
                    !ttParam->getDefaultArgument().getType()->isDependentType()) {
                    nonDepParam = ttParam;
                    for (const auto *p : alias->getTemplateParameters()->asArray()) {
                        if (auto *t = dyn_cast<TemplateTypeParmDecl>(p)) {
                            if (!t->hasDefaultArgument()) {
                                existingParam = t;
                                break;
                            }
                        }
                    }
                    break;
                }
            }
        }

        if (!nonDepParam || !existingParam)
            return;

        SourceLocation templateStart = alias->getTemplateParameters()->getBeginLoc();
        SourceLocation templateEnd = alias->getTemplateParameters()->getEndLoc();
        std::string templateParams = stringutils::rangetoStr(*Result.SourceManager, SourceRange(templateStart, templateEnd));

        std::string newParam = ", typename " + existingParam->getNameAsString() + "_dep = " + existingParam->getNameAsString();
        std::string newTemplateParams = templateParams + newParam;

        Rewrite.ReplaceText(SourceRange(templateStart, templateEnd), newTemplateParams);

        auto *aliasDecl = alias->getTemplatedDecl();
        if (auto *tsi = aliasDecl->getTypeSourceInfo()) {
            SourceRange typeRange = tsi->getTypeLoc().getSourceRange();
            std::string typeStr = stringutils::rangetoStr(*Result.SourceManager, typeRange);
            std::string oldArg = nonDepParam->getDefaultArgument().getType().getAsString();
            std::string newArg = existingParam->getNameAsString() + "_dep";
            size_t pos = typeStr.find(oldArg);
            if (pos != std::string::npos) {
                std::string modifiedTypeStr = typeStr;
                modifiedTypeStr.replace(pos, oldArg.length(), newArg);
                Rewrite.ReplaceText(typeRange, modifiedTypeStr);
            }
        }
    }
}
  
void MutatorFrontendAction_407::MutatorASTConsumer_407::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasTemplateDecl(
        hasAnyTemplateParameter(
            templateTypeParmDecl(hasDefaultArgument())
        )
    ).bind("alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}