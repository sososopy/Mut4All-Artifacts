//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_190
 */ 
class MutatorFrontendAction_190 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(190)

private:
    class MutatorASTConsumer_190 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_190(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> targetFunctions;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_190.h"

// ========================================================================================================
#define MUT190_OUTPUT 1

void MutatorFrontendAction_190::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        if (!FD->isThisDeclarationADefinition())
            return;

        //Record the target function for mutation
        targetFunctions.push_back(FD);
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TD->getLocation()))
            return;

        //Get the source code text of target node
        auto templateName = TD->getNameAsString();
        if (templateName.empty())
            return;

        //Perform mutation on the source code text by applying string replacement
        std::string recursiveTemplateDef = "template<int N>\nstruct " + templateName + " {\n"
                                           "  static const int value = " + templateName + "<N-1>::value + 1;\n"
                                           "};\n"
                                           "template<>\nstruct " + templateName + "<0> {\n"
                                           "  static const int value = 0;\n"
                                           "};\n";

        //Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(TD->getEndLoc(), "\n/*mut190*/" + recursiveTemplateDef);
    }
}

void MutatorFrontendAction_190::MutatorASTConsumer_190::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    auto functionMatcher = functionDecl(isDefinition()).bind("Functions");
    auto templateMatcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}