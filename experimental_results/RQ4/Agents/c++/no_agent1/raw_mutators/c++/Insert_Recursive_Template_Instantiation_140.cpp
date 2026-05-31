//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_140
 */ 
class MutatorFrontendAction_140 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(140)
private:
    class MutatorASTConsumer_140 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_140(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_recursive_template_instantiation_140.h"

// ========================================================================================================
#define MUT140_OUTPUT 1

void MutatorFrontendAction_140::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (TD->isThisDeclarationADefinition()) {
            auto templateName = TD->getNameAsString();
            std::string recursiveTemplate = "template<int N> struct " + templateName + " {\n";
            recursiveTemplate += "static const int value = " + templateName + "<N-1>::value;\n";
            recursiveTemplate += "};\n";
            recursiveTemplate += "template<> struct " + templateName + "<0> {\n";
            recursiveTemplate += "static const int value = 0;\n";
            recursiveTemplate += "};\n";
            recursiveTemplate = "/*mut140*/" + recursiveTemplate;

            Rewrite.InsertTextAfter(TD->getEndLoc(), recursiveTemplate);
        }
    }
}

void MutatorFrontendAction_140::MutatorASTConsumer_140::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}