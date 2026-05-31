//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Function_Instantiation_472
 */ 
class MutatorFrontendAction_472 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(472)

private:
    class MutatorASTConsumer_472 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_472(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> templateFunctions;
    };
};

//source file
#include "../include/Template_Function_Instantiation_472.h"

// ========================================================================================================
#define MUT472_OUTPUT 1

void MutatorFrontendAction_472::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunction")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation()) {
            templateFunctions.push_back(FD);
        }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionCall")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (!templateFunctions.empty()) {
            auto *templateFunction = templateFunctions.back();
            std::string instantiationText = "/*mut472*/" + templateFunction->getNameAsString() + "<LargeStruct>(LargeStructInstance);\n";
            instantiationText += templateFunction->getNameAsString() + "<NestedVector>(NestedVectorInstance);\n";
            Rewrite.InsertTextAfterToken(FD->getEndLoc(), instantiationText);
        }
    }
}

void MutatorFrontendAction_472::MutatorASTConsumer_472::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto templateMatcher = functionDecl(isTemplateInstantiation()).bind("TemplateFunction");
    auto functionCallMatcher = functionDecl(hasDescendant(callExpr())).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(functionCallMatcher, &callback);
    matchFinder.matchAST(Context);
}