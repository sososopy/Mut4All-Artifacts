//header file
#pragma once
#include "Mutator_base.h"

/**
 * Convert_NonTemplated_Function_To_Template_406
 */ 
class MutatorFrontendAction_406 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(406)

private:
    class MutatorASTConsumer_406 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_406(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Convert_NonTemplated_Function_To_Template_406.h"

// ========================================================================================================
#define MUT406_OUTPUT 1

void MutatorFrontendAction_406::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        // Skip if it's a template
        if (FD->isTemplated())
            return;

        // Get the source text of the function
        auto funcText = stringutils::rangetoStr(*Result.SourceManager, FD->getSourceRange());

        // Find the position of the opening brace
        size_t openBracePos = funcText.find("{");
        if (openBracePos == string::npos) {
            // No body, just add template
            string newDecl = "template<typename T>\n" + funcText;
            Rewrite.ReplaceText(FD->getSourceRange(), newDecl);
        } else {
            // Replace body with semicolon and add template
            string declPart = funcText.substr(0, openBracePos);
            string newDecl = "template<typename T>\n" + declPart + ";";
            Rewrite.ReplaceText(FD->getSourceRange(), newDecl);
        }
    }
}

void MutatorFrontendAction_406::MutatorASTConsumer_406::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify non-templated function declarations
    DeclarationMatcher matcher = functionDecl(unless(isTemplated())).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}