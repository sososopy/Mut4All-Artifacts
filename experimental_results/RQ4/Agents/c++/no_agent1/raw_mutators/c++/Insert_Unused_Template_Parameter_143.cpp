//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Template_Parameter_143
 */ 
class MutatorFrontendAction_143 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(143)

private:
    class MutatorASTConsumer_143 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_143(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::FunctionDecl *> processedFunctions;
    };
};

//source file
#include "../include/insert_unused_template_parameter_143.h"

// ========================================================================================================
#define MUT143_OUTPUT 1

void MutatorFrontendAction_143::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (processedFunctions.find(FD) != processedFunctions.end())
            return;

        processedFunctions.insert(FD);

        if (FD->isTemplateInstantiation())
            return;

        if (FD->getTemplatedKind() != FunctionDecl::TK_NonTemplate)
            return;

        auto functionText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

        std::string templateParam = "template<typename UnusedType>\n";
        functionText.insert(0, templateParam);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);
    }
}

void MutatorFrontendAction_143::MutatorASTConsumer_143::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isExpansionInSystemHeader())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}