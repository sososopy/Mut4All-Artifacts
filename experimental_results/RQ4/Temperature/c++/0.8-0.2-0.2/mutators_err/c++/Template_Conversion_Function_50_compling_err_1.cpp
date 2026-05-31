```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_conversion_function_50
 */ 
class MutatorFrontendAction_50 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(50)

private:
    class MutatorASTConsumer_50 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_50(Rewriter &R) : TheRewriter(R) {}
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
        const CXXMethodDecl *currentMethod = nullptr;
    };
};

//source file
#include "../include/template_conversion_function_50.h"

// ========================================================================================================
#define MUT50_OUTPUT 1

void MutatorFrontendAction_50::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (const auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Method")) {
        //Filter nodes in header files
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        if (MT->hasBody()) {
            //Get the source code text of target node
            auto bodyRange = MT->getBody()->getSourceRange();
            auto bodyText = Lexer::getSourceText(CharSourceRange::getTokenRange(bodyRange), *Result.SourceManager, Result.Context->getLangOpts());

            //Perform mutation on the source code text by applying string replacement
            std::string newBodyText = "{\n/*mut50*/(true ? 0 : operator int());\n}";

            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(bodyRange, newBodyText);
        }
    }
}

void MutatorFrontendAction_50::MutatorASTConsumer_50::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(ofClass(cxxRecordDecl(isTemplateInstantiation()))).bind("Method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

```