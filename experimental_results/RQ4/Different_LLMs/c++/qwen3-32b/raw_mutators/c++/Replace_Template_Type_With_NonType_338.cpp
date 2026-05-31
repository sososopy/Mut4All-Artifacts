//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Type_With_NonType_338
 */ 
class MutatorFrontendAction_338 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(338)

private:
    class MutatorASTConsumer_338 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_338(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Type_With_NonType_338.h"

// ========================================================================================================
#define MUT338_OUTPUT 1

void MutatorFrontendAction_338::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ATD = Result.Nodes.getNodeAs<clang::AliasTemplateDecl>("AliasTemplate")) {
        if (!ATD || !Result.Context->getSourceManager().isWrittenInMainFile(ATD->getLocation()))
            return;
        
        TemplateParameterList *Params = ATD->getTemplateParameters();
        if (!Params || Params->size() == 0)
            return;
        
        TemplateTypeParmDecl *TypeParam = cast<TemplateTypeParmDecl>(Params->getParam(0));
        SourceRange ParamRange = TypeParam->getSourceRange();
        
        std::string NewParam = "auto";
        if (TypeParam->hasName()) {
            NewParam += " " + TypeParam->getNameAsString();
        }
        
        Rewrite.ReplaceText(ParamRange, NewParam);
    }
}

void MutatorFrontendAction_338::MutatorASTConsumer_338::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = aliasTemplateDecl(
        has(templateParameterList(
            has(typedTemplateParm(decl()))
        ))
    ).bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}