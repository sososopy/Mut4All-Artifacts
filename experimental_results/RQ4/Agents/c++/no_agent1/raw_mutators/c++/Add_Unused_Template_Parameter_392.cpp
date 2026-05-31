//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Unused_Template_Parameter_392
 */ 
class MutatorFrontendAction_392 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(392)

private:
    class MutatorASTConsumer_392 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_392(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/add_unused_template_parameter_392.h"

// ========================================================================================================
#define MUT392_OUTPUT 1

void MutatorFrontendAction_392::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation() || FD->getTemplatedKind() != clang::FunctionDecl::TK_NonTemplate)
            return;

        auto functionText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

        std::string templateParam = "template<typename UnusedParam>\n";
        functionText.insert(0, templateParam);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);
    }
}

void MutatorFrontendAction_392::MutatorASTConsumer_392::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}