//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Template_Instantiation_163
 */ 
class MutatorFrontendAction_163 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(163)

private:
    class MutatorASTConsumer_163 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_163(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unused_template_instantiation_163.h"

// ========================================================================================================
#define MUT163_OUTPUT 1

void MutatorFrontendAction_163::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto funcName = FD->getNameAsString();
            std::string templateCode = "template<typename T> struct UnusedTemplate { T value; };";
            std::string instantiationCode = "UnusedTemplate<int> unusedInstance;\n";

            auto funcBodyRange = FD->getBody()->getSourceRange();
            std::string funcBodyText = stringutils::rangetoStr(*(Result.SourceManager), funcBodyRange);

            funcBodyText.insert(funcBodyText.find("{") + 1, "\n/*mut163*/" + templateCode + "\n" + instantiationCode);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(funcBodyRange), funcBodyText);
        }
    }
}

void MutatorFrontendAction_163::MutatorASTConsumer_163::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}