//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_376
 */ 
class MutatorFrontendAction_376 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(376)
private:
    class MutatorASTConsumer_376 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_376(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_recursive_template_instantiation_376.h"

// ========================================================================================================
#define MUT376_OUTPUT 1

void MutatorFrontendAction_376::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                    CTD->getLocation()))
            return;
        if (!CTD->isThisDeclarationADefinition())
            return;

        templates.push_back(CTD);

        // Get the source code text of target node
        auto templateName = CTD->getNameAsString();
        std::string recursiveTemplate = "template<int N> struct " + templateName + " {\n";
        recursiveTemplate += "    " + templateName + "<N-1> next;\n";
        recursiveTemplate += "};\n";
        recursiveTemplate += "template<> struct " + templateName + "<0> {};\n";

        // Perform mutation on the source code text by applying string replacement
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CTD->getSourceRange()), recursiveTemplate);
    }
}

void MutatorFrontendAction_376::MutatorASTConsumer_376::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}