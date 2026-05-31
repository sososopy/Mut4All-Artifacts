//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Template_Parameter_341
 */ 
class MutatorFrontendAction_341 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(341)

private:
    class MutatorASTConsumer_341 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_341(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::FunctionDecl *> visitedFunctions;
    };
};

//source file
#include "../include/insert_unused_template_parameter_341.h"

// ========================================================================================================
#define MUT341_OUTPUT 1

void MutatorFrontendAction_341::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (visitedFunctions.find(FD) != visitedFunctions.end())
            return;
        
        visitedFunctions.insert(FD);

        if (FD->isTemplateInstantiation() || FD->isFunctionTemplateSpecialization())
            return;

        // Get the source code text of target node
        auto functionText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string templateParam = "template<typename UnusedType>";
        functionText.insert(0, templateParam + "\n");

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);
    }
}

void MutatorFrontendAction_341::MutatorASTConsumer_341::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}