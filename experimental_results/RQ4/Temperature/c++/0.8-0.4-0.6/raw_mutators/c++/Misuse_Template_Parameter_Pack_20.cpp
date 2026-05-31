//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_template_parameter_pack_20
 */ 
class MutatorFrontendAction_20 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(20)

private:
    class MutatorASTConsumer_20 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_20(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misuse_template_parameter_pack_20.h"

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        auto &SM = Result.Context->getSourceManager();
        auto &LangOpts = Result.Context->getLangOpts();

        auto templateParams = FD->getTemplateParameters();
        bool hasParamPack = false;
        for (auto param : *templateParams) {
            if (param->isParameterPack()) {
                hasParamPack = true;
                break;
            }
        }

        if (!hasParamPack) {
            std::string replacement = "template <typename T, typename... Args>";
            auto range = FD->getSourceRange();
            std::string originalCode = Lexer::getSourceText(CharSourceRange::getTokenRange(range), SM, LangOpts).str();
            size_t insertPos = originalCode.find("template <typename T>");
            if (insertPos != std::string::npos) {
                originalCode.replace(insertPos, 19, replacement);
                size_t funcBodyPos = originalCode.find("{");
                if (funcBodyPos != std::string::npos) {
                    originalCode.insert(funcBodyPos + 1, "\n/*mut20*/ someFunction(args...);");
                }
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(range), originalCode);
            }
        }
    }
}

void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}