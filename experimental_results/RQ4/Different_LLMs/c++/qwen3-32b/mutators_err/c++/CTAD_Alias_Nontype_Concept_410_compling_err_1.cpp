//header file
#pragma once
#include "Mutator_base.h"

/**
 * CTAD_Alias_Nontype_Concept_410
 */ 
class MutatorFrontendAction_410 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(410)

private:
    class MutatorASTConsumer_410 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_410(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/CTAD_Alias_Nontype_Concept_410.h"

// ========================================================================================================
#define MUT410_OUTPUT 1

void MutatorFrontendAction_410::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::AliasTemplateDecl>("AliasTemplate")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        auto *usingDecl = MT->getTemplatedDecl();
        if (!usingDecl || !isa<clang::UsingDecl>(usingDecl))
            return;

        auto aliasName = usingDecl->getNameAsString();
        auto underlyingType = usingDecl->getTypeSourceInfo()->getType().getAsString();

        auto aliasSource = stringutils::rangetoStr(*Result.SourceManager, MT->getSourceRange());

        size_t openAngle = underlyingType.find('<');
        if (openAngle == string::npos)
            return;

        string templateName = underlyingType.substr(0, openAngle);
        string args = underlyingType.substr(openAngle + 1, underlyingType.find('>') - openAngle - 1);

        string newArgs = "decltype(V), std::integral<decltype(V)> ? 42 : V";
        string newUnderlyingType = templateName + "<" + newArgs + ">";

        string newTemplateParams = "auto V";
        string newAliasCode = "template <" + newTemplateParams + "> using " + aliasName + " = " + newUnderlyingType + ";";

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), newAliasCode);
    }
}
  
void MutatorFrontendAction_410::MutatorASTConsumer_410::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = aliasTemplateDecl(has(templateParams(1, typeTemplateParm()))).bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}