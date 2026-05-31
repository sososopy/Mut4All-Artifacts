//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_126
 */ 
class MutatorFrontendAction_126 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(126)

private:
    class MutatorASTConsumer_126 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_126(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> visitedTemplates;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_126.h"

// ========================================================================================================
#define MUT126_OUTPUT 1

void MutatorFrontendAction_126::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        auto templateName = TD->getNameAsString();
        if (visitedTemplates.find(templateName) != visitedTemplates.end())
            return;
        
        visitedTemplates.insert(templateName);

        auto templateSrc = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());

        std::string recursiveInstance = "\n/*mut126*/template<>\nstruct " + templateName + "<0> {\n"
                                        "  using type = " + templateName + "<0>;\n};\n";

        if (templateSrc.rfind('}') != std::string::npos)
            templateSrc.insert(templateSrc.rfind('}'), recursiveInstance);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateSrc);
    }
}

void MutatorFrontendAction_126::MutatorASTConsumer_126::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}