//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_default_template_arguments_35
 */ 
class MutatorFrontendAction_35 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(35)

private:
    class MutatorASTConsumer_35 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_35(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_default_template_arguments_35.h"

// ========================================================================================================
#define MUT35_OUTPUT 1

void MutatorFrontendAction_35::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplates")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;

        for (auto *param : FT->getTemplateParameters()->asArray()) {
            if (auto *typeParam = dyn_cast<clang::TemplateTypeParmDecl>(param)) {
                if (typeParam->hasDefaultArgument()) {
                    auto defaultArg = typeParam->getDefaultArgument().getAsType().getAsString();
                    std::string newDefaultArg = "NonExistentType";
                    std::string sourceText = stringutils::rangetoStr(*(Result.SourceManager), typeParam->getSourceRange());
                    size_t pos = sourceText.find(defaultArg);
                    if (pos != std::string::npos) {
                        sourceText.replace(pos, defaultArg.length(), newDefaultArg);
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(typeParam->getSourceRange()), sourceText);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_35::MutatorASTConsumer_35::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateTypeParmDecl(hasDefaultArgument()))).bind("FunctionTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}