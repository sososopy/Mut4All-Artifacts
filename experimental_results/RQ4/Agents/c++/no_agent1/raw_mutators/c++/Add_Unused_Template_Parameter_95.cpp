//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Unused_Template_Parameter_95
 */ 
class MutatorFrontendAction_95 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(95)

private:
    class MutatorASTConsumer_95 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_95(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/add_unused_template_parameter_95.h"

// ========================================================================================================
#define MUT95_OUTPUT 1

void MutatorFrontendAction_95::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (!FD->isTemplated()) {
            auto funcSourceRange = FD->getSourceRange();
            std::string funcText = stringutils::rangetoStr(*(Result.SourceManager), funcSourceRange);

            std::string templateParam = "template<typename UnusedType>\n";
            funcText = templateParam + funcText;

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(funcSourceRange), funcText);
        }
    }
}

void MutatorFrontendAction_95::MutatorASTConsumer_95::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isTemplateInstantiation())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}