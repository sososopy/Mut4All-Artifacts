//header file
#pragma once
#include "Mutator_base.h"

/**
 * Convert_Template_TypeAlias_To_Variadic_29
 */ 
class MutatorFrontendAction_29 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(29)
private:
    class MutatorASTConsumer_29 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_29(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Convert_Template_TypeAlias_To_Variadic_29.h"

// ========================================================================================================
#define MUT29_OUTPUT 1

void MutatorFrontendAction_29::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TAT = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("TypeAlias")) {
        if (!TAT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TAT->getLocation()))
            return;

        // Get template parameters source range
        SourceRange paramsRange = TAT->getTemplateParameters()->getSourceRange();
        std::string originalTemplateParams = stringutils::rangetoStr(*Result.SourceManager, paramsRange);

        // Convert to variadic
        std::string newTemplateParams = originalTemplateParams;
        size_t pos = newTemplateParams.find("typename ");
        if (pos != std::string::npos) {
            newTemplateParams.replace(pos, 7, "typename...");
            Rewrite.ReplaceText(paramsRange, newTemplateParams);
        }

        // Modify type alias definition
        auto *TA = cast<TypeAliasDecl>(TAT->getTemplatedDecl());
        SourceRange typeRange = TA->getTypeSourceInfo()->getTypeLoc().getSourceRange();
        std::string originalType = stringutils::rangetoStr(*Result.SourceManager, typeRange);

        // Insert pack parameter at first comma
        size_t firstComma = originalType.find(',');
        if (firstComma != std::string::npos) {
            std::string newType = originalType.substr(0, firstComma + 1) + "T, " + originalType.substr(firstComma + 1);
            Rewrite.ReplaceText(typeRange, newType);
        }
    }
}

void MutatorFrontendAction_29::MutatorASTConsumer_29::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasTemplateDecl(has(templateParameters(hasSize(1)))).bind("TypeAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}