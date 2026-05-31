//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Template_Parameter_183
 */ 
class MutatorFrontendAction_183 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(183)

private:
    class MutatorASTConsumer_183 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_183(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unused_template_parameter_183.h"

// ========================================================================================================
#define MUT183_OUTPUT 1

void MutatorFrontendAction_183::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (!FD->isTemplateInstantiation() && FD->getTemplatedKind() == clang::FunctionDecl::TK_NonTemplate) {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            size_t pos = declaration.find("(");
            if (pos != std::string::npos) {
                std::string templateParam = "template<typename UnusedType> ";
                declaration.insert(0, templateParam);
                declaration.insert(pos, "<UnusedType>");
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
            }
        }
    }
}

void MutatorFrontendAction_183::MutatorASTConsumer_183::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}