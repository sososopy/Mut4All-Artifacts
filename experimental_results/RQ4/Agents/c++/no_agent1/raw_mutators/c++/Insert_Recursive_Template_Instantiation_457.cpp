//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_457
 */ 
class MutatorFrontendAction_457 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(457)

private:
    class MutatorASTConsumer_457 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_457(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_recursive_template_instantiation_457.h"

// ========================================================================================================
#define MUT457_OUTPUT 1

void MutatorFrontendAction_457::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (!TD->isThisDeclarationADefinition())
            return;

        auto templateName = TD->getNameAsString();
        std::string recursiveTemplate = 
            "template<int N>\n"
            "struct " + templateName + "_Recursive {\n"
            "    static const int value = " + templateName + "_Recursive<N-1>::value;\n"
            "};\n"
            "template<>\n"
            "struct " + templateName + "_Recursive<0> {\n"
            "    static const int value = 0;\n"
            "};\n";

        Rewrite.InsertTextAfter(TD->getEndLoc(), "\n/*mut457*/\n" + recursiveTemplate);
    }
}

void MutatorFrontendAction_457::MutatorASTConsumer_457::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}