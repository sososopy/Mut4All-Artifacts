//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Template_Parameter_198
 */ 
class MutatorFrontendAction_198 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(198)
private:
    class MutatorASTConsumer_198 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_198(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unused_template_parameter_198.h"

// ========================================================================================================
#define MUT198_OUTPUT 1

void MutatorFrontendAction_198::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FD->getLocation()))
            return;

        if (FD->getTemplatedKind() != FunctionDecl::TK_NonTemplate)
            return;

        std::string funcText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
        std::string templateParam = "template<typename UnusedParam>\n";
        funcText.insert(0, templateParam);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcText);
    }
}

void MutatorFrontendAction_198::MutatorASTConsumer_198::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isTemplateInstantiation())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}