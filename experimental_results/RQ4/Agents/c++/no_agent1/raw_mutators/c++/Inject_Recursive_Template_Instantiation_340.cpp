//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inject_Recursive_Template_Instantiation_340
 */ 
class MutatorFrontendAction_340 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(340)

private:
    class MutatorASTConsumer_340 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_340(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/inject_recursive_template_instantiation_340.h"

// ========================================================================================================
#define MUT340_OUTPUT 1

void MutatorFrontendAction_340::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (!TD->isThisDeclarationADefinition())
            return;

        auto templateName = TD->getNameAsString();
        std::string recursiveTemplate = "template<int N> struct " + templateName + " { enum { value = " + templateName + "<N-1>::value }; };\n";
        recursiveTemplate += "template<> struct " + templateName + "<0> { enum { value = 0 }; };\n";
        recursiveTemplate = "/*mut340*/" + recursiveTemplate;

        Rewrite.InsertTextAfterToken(TD->getEndLoc(), recursiveTemplate);
    }
}

void MutatorFrontendAction_340::MutatorASTConsumer_340::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}