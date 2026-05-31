//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_With_Template_226
 */ 
class MutatorFrontendAction_226 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(226)

private:
    class MutatorASTConsumer_226 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_226(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_function_with_template_226.h"

// ========================================================================================================
#define MUT226_OUTPUT 1

void MutatorFrontendAction_226::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation() || FD->isDeleted() || FD->isDefaulted())
            return;

        auto functionName = FD->getNameAsString();
        auto returnType = FD->getReturnType().getAsString();
        std::string templateText = "template<typename T>\n" + returnType + " " + functionName + "(T param)";

        auto functionSource = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
        size_t pos = functionSource.find(functionName);
        if (pos != std::string::npos) {
            functionSource.replace(pos, functionName.length(), templateText);
            functionSource.insert(functionSource.find("{") + 1, "\n/*mut226*/");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionSource);
        }
    }
}
  
void MutatorFrontendAction_226::MutatorASTConsumer_226::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isTemplateInstantiation())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}