//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_120
 */ 
class MutatorFrontendAction_120 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(120)

private:
    class MutatorASTConsumer_120 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_120(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> templates;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_120.h"

// ========================================================================================================
#define MUT120_OUTPUT 1

void MutatorFrontendAction_120::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CTD->getLocation()))
            return;
        if (!CTD->isThisDeclarationADefinition())
            return;

        templates.push_back(CTD);

        // Get the source code text of target node
        auto templateName = CTD->getNameAsString();
        auto templateSource = stringutils::rangetoStr(*(Result.SourceManager), CTD->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string recursiveInstantiation = "\n/*mut120*/template<> struct " + templateName + "<0> { using type = " + templateName + "<0>; };";
        templateSource += recursiveInstantiation;

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CTD->getSourceRange()), templateSource);
    }
}

void MutatorFrontendAction_120::MutatorASTConsumer_120::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}