```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Instantiation_Mismatch_145
 */ 
class MutatorFrontendAction_145 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(145)

private:
    class MutatorASTConsumer_145 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_145(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_instantiation_mismatch_145.h"

// ========================================================================================================
#define MUT145_OUTPUT 1

void MutatorFrontendAction_145::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
        //Filter nodes in header files
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
            return;

        //Get the source code text of target node
        auto funcTemplateText = stringutils::rangetoStr(
            *(Result.SourceManager), FTD->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        size_t templatePos = funcTemplateText.find("template<");
        if (templatePos != std::string::npos) {
            size_t endTemplatePos = funcTemplateText.find(">", templatePos);
            if (endTemplatePos != std::string::npos) {
                funcTemplateText.insert(endTemplatePos, ", typename U");
            }
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), funcTemplateText);
    }
}

void MutatorFrontendAction_145::MutatorASTConsumer_145::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
```