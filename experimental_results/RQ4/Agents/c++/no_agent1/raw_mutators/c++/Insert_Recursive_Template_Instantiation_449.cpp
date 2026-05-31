//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_449
 */ 
class MutatorFrontendAction_449 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(449)

private:
    class MutatorASTConsumer_449 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_449(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_recursive_template_instantiation_449.h"

// ========================================================================================================
#define MUT449_OUTPUT 1

void MutatorFrontendAction_449::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (!TD->isThisDeclarationADefinition())
            return;

        // Get the source code text of target node
        auto templateSource = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());

        // Perform mutation by inserting a recursive template instantiation
        std::string recursiveTemplate = R"(
            template<int N>
            struct RecursiveTemplate {
                static const int value = RecursiveTemplate<N-1>::value + 1;
            };

            template<>
            struct RecursiveTemplate<0> {
                static const int value = 0;
            };

            /*mut449*/ int recursiveValue = RecursiveTemplate<100>::value;
        )";

        if (templateSource.rfind('}') != std::string::npos) {
            templateSource.insert(templateSource.rfind('}'), recursiveTemplate);
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateSource);
    }
}

void MutatorFrontendAction_449::MutatorASTConsumer_449::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}